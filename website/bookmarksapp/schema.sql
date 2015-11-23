CREATE TABLE `bookmarks` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `hash` varchar(32) collate utf8_unicode_ci NOT NULL default '',
  `url` text collate utf8_unicode_ci NOT NULL,
  `title` text collate utf8_unicode_ci NOT NULL,
  `dt` timestamp NOT NULL default CURRENT_TIMESTAMP,
  PRIMARY KEY  (`id`),
  UNIQUE KEY `hash` (`hash`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
