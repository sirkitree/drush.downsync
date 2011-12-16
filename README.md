Drush Downsync
==============

Move one environment's database and files directory to another.

This script currently relies on Drush* and Drush's site alias features. And now
that it's using drush_invoke() instead of drush_shell_exec(), you do not need
a 'files' key in your site alias. core-rsync has a %files path, so as long as
your remote site is reporting that path correctly, this should be able to grab
your remote files.

It also will setup dump file locations (source-dump and target-dump) for you if
they are not already setup, however, you should still be able to override these
options by specifying them yourself, as well as any other options supported by
sql-sync.

* http://drupal.org/project/drush