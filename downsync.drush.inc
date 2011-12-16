<?php

/**
 * @file
 * Downsync
 */

/**
 * Implements hook_drush_command().
 */
function downsync_drush_command() {
  $items = array();
  $items['downsync'] = array(
    'description' => "Sync one environment's db and files to another. You will NEED to add a 'files' value to your site aliases.",
    'arguments' => array(
      'source-alias' => 'Alias of the source site to copy from.',
      'destination-alias' => 'Alias of the destination site to copy to.',
    ),
    'examples' => array(
      'drush downsync @prod @local' => 'sql-sync the database and rsync the files from @prod to @local',
    ),
    'aliases' => array('dsync'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'drush dependencies' => array('sql'),
  );
  return $items;
}

function drush_downsync_validate($source = NULL, $destination = NULL) {
  $source_settings = drush_sitealias_get_record($source);
  $destination_settings = drush_sitealias_get_record($destination);

  if (!isset($source_settings['files']) || !isset($destination_settings['files'])) {
    return drush_log(dt('You MUST add a "files" key to your aliases with a value of the absolute path of the files directory, excluding trailing slash.'), 'failed');
  }
}

function drush_downsync($source = NULL, $destination = NULL) {
  // Execute a drush sql-sync
  print dt('SQL Sync running... NOTE: if you do not have ssh passwordless logins setup, you may be asked for your password multiple times.');
  drush_shell_exec('drush sql-sync '. $source .' '. $destination .' --source-dump=/tmp/dump.sql --target-dump=/tmp/dump.sql -y');

  // Rsync the files.
  // This assumes that you have the absolute file paths in your aliases.
  $source_settings = drush_sitealias_get_record($source);
  $remote_user = $source_settings['remote-user'];
  $remote_host = $source_settings['remote-host'];
  $remote_files = $source_settings['files'];

  $destination_settings = drush_sitealias_get_record($destination);

  $exec = 'rsync -rv '. $remote_user .'@'. $remote_host .'://'. $remote_files .'/* '. $destination_settings['files'];
  // print $exec;

  print dt('Rsync-ing files...');
  drush_shell_exec($exec);
}
