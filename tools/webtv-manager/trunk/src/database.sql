DROP TABLE IF EXISTS `wtvmT_AuthItem`;
DROP TABLE IF EXISTS `wtvmT_Lang`;
DROP TABLE IF EXISTS `wtvmT_AuthItemChild`;
DROP TABLE IF EXISTS `wtvmT_Country`;
DROP TABLE IF EXISTS `wtvmT_History`;
DROP TABLE IF EXISTS `wtvmT_StreamStatus`;
DROP TABLE IF EXISTS `wtvmT_TVChannel`;
DROP TABLE IF EXISTS `wtvmT_TVChannelType`;
DROP TABLE IF EXISTS `wtvmT_WebStream`;
DROP TABLE IF EXISTS `wtvmT_Action`;
DROP TABLE IF EXISTS `wtvmT_Activities`;
DROP TABLE IF EXISTS `wtvmT_Friendship`;
DROP TABLE IF EXISTS `wtvmT_Membership`;
DROP TABLE IF EXISTS `wtvmT_Messages`;
DROP TABLE IF EXISTS `wtvmT_Payment`;
DROP TABLE IF EXISTS `wtvmT_Permission`;
DROP TABLE IF EXISTS `wtvmT_PrivacySetting`;
DROP TABLE IF EXISTS `wtvmT_Profile_Comment`;
DROP TABLE IF EXISTS `wtvmT_Profile_Fields`;
DROP TABLE IF EXISTS `wtvmT_Profile_Fields_Group`;
DROP TABLE IF EXISTS `wtvmT_Profile_Visit`;
DROP TABLE IF EXISTS `wtvmT_Profiles`;
DROP TABLE IF EXISTS `wtvmT_Roles`;
DROP TABLE IF EXISTS `wtvmT_User_Has_Role`;
DROP TABLE IF EXISTS `wtvmT_UserGroup`;
DROP TABLE IF EXISTS `wtvmT_YumSettings`;
DROP TABLE IF EXISTS `wtvmT_Users`;
DROP TABLE IF EXISTS `wtvmT_YumTextSettings`;
DROP TABLE IF EXISTS `wtvmT_User_Has_Usergroup`;
DROP TABLE IF EXISTS `wtvmT_AuthAssignment`;


CREATE TABLE `wtvmT_Users` (
  `id` INTEGER NOT NULL AUTO_INCREMENT,
  `username` varchar(20) NOT NULL,
  `password` varchar(128) NOT NULL,
  `activationKey` varchar(128) NOT NULL DEFAULT '',
  `createtime` int(10) NOT NULL DEFAULT '0',
  `lastvisit` int(10) NOT NULL DEFAULT '0',
  `lastpasswordchange` int(10) NOT NULL DEFAULT '0',
  `superuser` int(1) NOT NULL DEFAULT '0',
  `status` int(1) NOT NULL DEFAULT '0',
  `avatar` varchar(255) DEFAULT NULL,
  `notifyType` enum('None','Digest','Instant','Treshhold') DEFAULT 'Instant',
  PRIMARY KEY (`id`),
  UNIQUE KEY `username` (`username`),
  KEY `status` (`status`),
  KEY `superuser` (`superuser`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;


--
-- Dumping data for table `wtvmT_Users`
--

LOCK TABLES `wtvmT_Users` WRITE;

INSERT INTO `wtvmT_Users` VALUES (1,'admin','21232f297a57a5a743894a0e4a801fc3','',1295044722,1295047330,0,1,1,NULL,'Instant'),(2,'demo','fe01ce2a7fbac8fafaed7c982a04e229','',1295044722,1295048896,0,0,1,NULL,'Instant');

UNLOCK TABLES;

--
-- Table structure for table `User_Has_Usergroup`
--

CREATE TABLE `wtvmT_User_Has_Usergroup` (
  `user_id` int(10) unsigned NOT NULL,
  `group_id` int(10) unsigned NOT NULL,
  `jointime` int(11) NOT NULL,
  PRIMARY KEY (`user_id`,`group_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Table structure for table `wtvmT_AuthAssignment`
--


CREATE TABLE `wtvmT_AuthAssignment` (
  `itemname` varchar(64) NOT NULL,
  `userid` varchar(64) NOT NULL,
  `bizrule` text,
  `data` text,
  PRIMARY KEY (`itemname`,`userid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

--
-- Table structure for table `wtvmT_AuthItem`
--


CREATE TABLE `wtvmT_AuthItem` (
  `name` varchar(64) NOT NULL,
  `type` int(11) NOT NULL,
  `description` text,
  `bizrule` text,
  `data` text,
  PRIMARY KEY (`name`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

--
-- Table structure for table `wtvmT_AuthItemChild`
--


CREATE TABLE `wtvmT_AuthItemChild` (
  `parent` varchar(64) NOT NULL,
  `child` varchar(64) NOT NULL,
  PRIMARY KEY (`parent`,`child`),
  KEY `child` (`child`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

--
-- Table structure for table `wtvmT_Country`
--


CREATE TABLE `wtvmT_Country` (
  `Code` char(2) NOT NULL,
  `Label` varchar(20) NOT NULL,
  PRIMARY KEY (`Code`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `wtvmT_Country`
--

LOCK TABLES `wtvmT_Country` WRITE;

INSERT INTO `wtvmT_Country` VALUES ('ad','Andorra'),('ae','United Arab Emirates'),('af','Afghanistan'),('ag','Antigua and Barbuda'),('ai','Anguilla'),('al','Albania'),('am','Armenia'),('an','Netherlands Antilles'),('ao','Angola'),('aq','Antarctica'),('ar','Argentina'),('as','American Samoa'),('at','Austria'),('au','Australia'),('aw','Aruba'),('az','Azerbaijan'),('ba','Bosnia and Herzegovi'),('bb','Barbados'),('bd','Bangladesh'),('be','Belgium'),('bf','Burkina Faso'),('bg','Bulgaria'),('bh','Bahrain'),('bi','Burundi'),('bj','Benin'),('bm','Bermuda'),('bn','Brunei Darussalam'),('bo','Bolivia'),('br','Brazil'),('bs','Bahamas'),('bt','Bhutan'),('bv','Bouvet Island'),('bw','Botswana'),('by','Belarus'),('bz','Belize'),('ca','Canada'),('cc','Cocos (Keeling) Isla'),('cd','Congo, the Democrati'),('cf','Central African Repu'),('cg','Congo'),('ch','Switzerland'),('ci','Cote D\'Ivoire'),('ck','Cook Islands'),('cl','Chile'),('cm','Cameroon'),('cn','China'),('co','Colombia'),('cr','Costa Rica'),('cs','Serbia and Montenegr'),('cu','Cuba'),('cv','Cape Verde'),('cx','Christmas Island'),('cy','Cyprus'),('cz','Czech Republic'),('de','Germany'),('dj','Djibouti'),('dk','Denmark'),('dm','Dominica'),('do','Dominican Republic'),('dz','Algeria'),('ec','Ecuador'),('ee','Estonia'),('eg','Egypt'),('eh','Western Sahara'),('er','Eritrea'),('es','Spain'),('et','Ethiopia'),('fi','Finland'),('fj','Fiji'),('fk','Falkland Islands (Ma'),('fm','Micronesia, Federate'),('fo','Faroe Islands'),('fr','France'),('ga','Gabon'),('gb','United Kingdom'),('gd','Grenada'),('ge','Georgia'),('gf','French Guiana'),('gh','Ghana'),('gi','Gibraltar'),('gl','Greenland'),('gm','Gambia'),('gn','Guinea'),('gp','Guadeloupe'),('gq','Equatorial Guinea'),('gr','Greece'),('gs','South Georgia and th'),('gt','Guatemala'),('gu','Guam'),('gw','Guinea-Bissau'),('gy','Guyana'),('hk','Hong Kong'),('hm','Heard Island and Mcd'),('hn','Honduras'),('hr','Croatia'),('ht','Haiti'),('hu','Hungary'),('id','Indonesia'),('ie','Ireland'),('il','Israel'),('in','India'),('io','British Indian Ocean'),('iq','Iraq'),('ir','Iran, Islamic Republ'),('is','Iceland'),('it','Italy'),('jm','Jamaica'),('jo','Jordan'),('jp','Japan'),('ke','Kenya'),('kg','Kyrgyzstan'),('kh','Cambodia'),('ki','Kiribati'),('km','Comoros'),('kn','Saint Kitts and Nevi'),('kp','Korea, Democratic Pe'),('kr','Korea, Republic of'),('kw','Kuwait'),('ky','Cayman Islands'),('kz','Kazakhstan'),('la','Lao People\'s Democra'),('lb','Lebanon'),('lc','Saint Lucia'),('li','Liechtenstein'),('lk','Sri Lanka'),('lr','Liberia'),('ls','Lesotho'),('lt','Lithuania'),('lu','Luxembourg'),('lv','Latvia'),('ly','Libyan Arab Jamahiri'),('ma','Morocco'),('mc','Monaco'),('md','Moldova, Republic of'),('mg','Madagascar'),('mh','Marshall Islands'),('mk','Macedonia, the Forme'),('ml','Mali'),('mm','Myanmar'),('mn','Mongolia'),('mo','Macao'),('mp','Northern Mariana Isl'),('mq','Martinique'),('mr','Mauritania'),('ms','Montserrat'),('mt','Malta'),('mu','Mauritius'),('mv','Maldives'),('mw','Malawi'),('mx','Mexico'),('my','Malaysia'),('mz','Mozambique'),('na','Namibia'),('nc','New Caledonia'),('ne','Niger'),('nf','Norfolk Island'),('ng','Nigeria'),('ni','Nicaragua'),('nl','Netherlands'),('no','Norway'),('np','Nepal'),('nr','Nauru'),('nu','Niue'),('nz','New Zealand'),('om','Oman'),('pa','Panama'),('pe','Peru'),('pf','French Polynesia'),('pg','Papua New Guinea'),('ph','Philippines'),('pk','Pakistan'),('pl','Poland'),('pm','Saint Pierre and Miq'),('pn','Pitcairn'),('pr','Puerto Rico'),('ps','Palestinian Territor'),('pt','Portugal'),('pw','Palau'),('py','Paraguay'),('qa','Qatar'),('re','Reunion'),('ro','Romania'),('ru','Russian Federation'),('rw','Rwanda'),('sa','Saudi Arabia'),('sb','Solomon Islands'),('sc','Seychelles'),('sd','Sudan'),('se','Sweden'),('sg','Singapore'),('sh','Saint Helena'),('si','Slovenia'),('sj','Svalbard and Jan May'),('sk','Slovakia'),('sl','Sierra Leone'),('sm','San Marino'),('sn','Senegal'),('so','Somalia'),('sr','Suriname'),('st','Sao Tome and Princip'),('sv','El Salvador'),('sy','Syrian Arab Republic'),('sz','Swaziland'),('tc','Turks and Caicos Isl'),('td','Chad'),('tf','French Southern Terr'),('tg','Togo'),('th','Thailand'),('tj','Tajikistan'),('tk','Tokelau'),('tl','Timor-Leste'),('tm','Turkmenistan'),('tn','Tunisia'),('to','Tonga'),('tr','Turkey'),('tt','Trinidad and Tobago'),('tv','Tuvalu'),('tw','Taiwan, Province of '),('tz','Tanzania, United Rep'),('ua','Ukraine'),('ug','Uganda'),('um','United States Minor '),('us','United States'),('uy','Uruguay'),('uz','Uzbekistan'),('va','Holy See (Vatican Ci'),('vc','Saint Vincent and th'),('ve','Venezuela'),('vg','Virgin Islands, Brit'),('vi','Virgin Islands, U.s.'),('vn','Viet Nam'),('vu','Vanuatu'),('wf','Wallis and Futuna'),('ws','Samoa'),('ye','Yemen'),('yt','Mayotte'),('za','South Africa'),('zm','Zambia'),('zw','Zimbabwe');

UNLOCK TABLES;

--
-- Table structure for table `wtvmT_History`
--

CREATE TABLE IF NOT EXISTS wtvmT_History (
	Id INTEGER NOT NULL AUTO_INCREMENT,
	Date DATETIME NOT NULL,
	Username VARCHAR(20) NULL,
	Email VARCHAR(255) NULL,
	UserId INTEGER NULL,
	RemoteAddr VARCHAR(40) NOT NULL,
	ActionType TINYINT NOT NULL,
	ActionDetails TEXT NULL,
	EntityType TINYINT NOT NULL,
	EntityId INTEGER NOT NULL,
	CONSTRAINT PK_History PRIMARY KEY(Id),
	CONSTRAINT FK_History_User FOREIGN KEY (UserId)
		REFERENCES wtvmT_Users(id)
) ENGINE=InnoDB CHARSET=utf8;


--
-- Dumping data for table `wtvmT_History`
--

LOCK TABLES `wtvmT_History` WRITE;

INSERT INTO `wtvmT_History` VALUES (1, '2010-10-12 00:28:10', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 769),(2, '2010-10-12 08:33:45', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 770),(3, '2010-10-17 21:55:11', NULL, NULL, 1, '82.235.90.160', 3, ' => WebLink', 1, 768),(4, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 761),(5, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 762),(6, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 763),(7, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 764),(8, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 765),(9, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 766),(10, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 767),(11, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 332),(12, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 333),(13, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 334),(14, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 335),(15, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 336),(16, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 337),(17, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 338),(18, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 339),(19, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 340),(20, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 341),(21, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 342),(22, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 343),(23, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 344),(24, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 345),(25, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 346),(26, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 369),(27, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 516),(28, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 517),(29, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 518),(30, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 519),(31, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 520),(32, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 521),(33, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 522),(34, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 523),(35, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 524),(36, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 525),(37, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 526),(38, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 527),(39, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 528),(40, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 529),(41, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 530),(42, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 531),(43, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 532),(44, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 533),(45, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 534),(46, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 535),(47, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 536),(48, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 537),(49, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 538),(50, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 539),(51, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 540),(52, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 541),(53, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 542),(54, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 543),(55, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 544),(56, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 545),(57, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 546),(58, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 547),(59, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 548),(60, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 549),(61, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 550),(62, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 551),(63, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 552),(64, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 553),(65, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 554),(66, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 555),(67, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 556),(68, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 557),(69, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 558),(70, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 559),(71, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 560),(72, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 561),(73, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 562),(74, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 563),(75, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 564),(76, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 565),(77, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 566),(78, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 567),(79, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 568),(80, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 569),(81, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 570),(82, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 571),(83, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 572),(84, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 573),(85, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 574),(86, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 575),(87, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 576),(88, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 577),(89, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 578),(90, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 579),(91, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 580),(92, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 581),(93, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 582),(94, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 583),(95, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 584),(96, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 585),(97, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 586),(98, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 587),(99, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 588),(100, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 589),(101, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 590),(102, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 591),(103, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 592),(104, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 593),(105, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 594),(106, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 595),(107, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 596),(108, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 597),(109, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 598),(110, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 599),(111, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 600),(112, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 601),(113, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 602),(114, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 603),(115, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 604),(116, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 605),(117, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 606),(118, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 607),(119, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 608),(120, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 609),(121, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 610),(122, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 611),(123, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 612),(124, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 613),(125, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 614),(126, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 615),(127, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 616),(128, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 617),(129, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 618),(130, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 619),(131, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 620),(132, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 621),(133, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 622),(134, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 623),(135, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 624),(136, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 625),(137, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 626),(138, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 627),(139, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 655),(140, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 372),(141, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 373),(142, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 374),(143, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 375),(144, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 376),(145, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 377),(146, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 378),(147, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 379),(148, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 380),(149, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 381),(150, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 382),(151, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 383),(152, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 384),(153, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 385),(154, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 386),(155, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 387),(156, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 388),(157, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 389),(158, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 390),(159, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 391),(160, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 392),(161, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 393),(162, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 394),(163, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 395),(164, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 396),(165, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 397),(166, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 398),(167, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 399),(168, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 400),(169, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 401),(170, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 402),(171, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 403),(172, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 404),(173, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 405),(174, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 406),(175, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 407),(176, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 408),(177, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 409),(178, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 410),(179, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 411),(180, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 412),(181, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 413),(182, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 414),(183, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 415),(184, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 416),(185, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 417),(186, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 418),(187, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 419),(188, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 420),(189, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 421),(190, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 422),(191, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 423),(192, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 424),(193, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 425),(194, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 426),(195, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 427),(196, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 428),(197, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 429),(198, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 430),(199, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 431),(200, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 432),(201, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 433),(202, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 434),(203, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 435),(204, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 436),(205, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 437),(206, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 438),(207, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 439),(208, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 440),(209, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 441),(210, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 442),(211, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 443),(212, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 444),(213, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 445),(214, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 446),(215, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 447),(216, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 448),(217, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 449),(218, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 450),(219, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 451),(220, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 452),(221, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 453),(222, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 454),(223, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 455),(224, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 456),(225, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 457),(226, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 458),(227, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 459),(228, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 460),(229, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 461),(230, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 462),(231, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 463),(232, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 464),(233, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 465),(234, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 466),(235, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 467),(236, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 468),(237, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 469),(238, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 470),(239, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 471),(240, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 472),(241, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 473),(242, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 474),(243, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 475),(244, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 476),(245, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 477),(246, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 478),(247, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 479),(248, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 480),(249, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 481),(250, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 482),(251, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 483),(252, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 484),(253, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 485),(254, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 486),(255, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 487),(256, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 488),(257, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 489),(258, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 490),(259, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 491),(260, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 492),(261, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 493),(262, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 494),(263, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 495),(264, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 496),(265, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 497),(266, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 498),(267, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 499),(268, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 500),(269, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 501),(270, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 502),(271, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 503),(272, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 504),(273, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 505),(274, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 506),(275, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 507),(276, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 508),(277, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 509),(278, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 510),(279, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 511),(280, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 512),(281, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 513),(282, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 514),(283, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 515),(284, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 689),(285, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 690),(286, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 691),(287, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 692),(288, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 693),(289, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 694),(290, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 695),(291, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 696),(292, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 697),(293, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 698),(294, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 699),(295, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 700),(296, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 701),(297, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 702),(298, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 703),(299, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 704),(300, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 705),(301, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 706),(302, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 707),(303, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 708),(304, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 709),(305, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 710),(306, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 711),(307, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 712),(308, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 713),(309, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 714),(310, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 715),(311, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 716),(312, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 717),(313, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 718),(314, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 719),(315, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 720),(316, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 721),(317, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 722),(318, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 723),(319, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 724),(320, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 725),(321, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 726),(322, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 727),(323, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 728),(324, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 729),(325, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 658),(326, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 659),(327, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 660),(328, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 661),(329, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 662),(330, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 1),(331, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 2),(332, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 3),(333, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 4),(334, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 5),(335, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 6),(336, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 7),(337, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 8),(338, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 9),(339, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 10),(340, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 11),(341, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 12),(342, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 13),(343, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 14),(344, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 15),(345, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 16),(346, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 17),(347, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 18),(348, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 19),(349, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 20),(350, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 21),(351, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 22),(352, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 23),(353, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 24),(354, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 25),(355, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 26),(356, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 27),(357, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 28),(358, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 29),(359, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 30),(360, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 31),(361, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 32),(362, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 33),(363, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 34),(364, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 35),(365, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 36),(366, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 37),(367, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 38),(368, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 39),(369, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 40),(370, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 41),(371, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 42),(372, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 43),(373, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 44),(374, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 45),(375, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 46),(376, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 47),(377, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 48),(378, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 49),(379, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 50),(380, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 51),(381, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 52),(382, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 53),(383, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 54),(384, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 55),(385, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 56),(386, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 57),(387, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 58),(388, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 59),(389, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 60),(390, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 61),(391, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 62),(392, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 63),(393, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 64),(394, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 65),(395, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 66),(396, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 67),(397, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 68),(398, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 69),(399, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 70),(400, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 71),(401, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 72),(402, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 73),(403, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 74),(404, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 75),(405, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 76),(406, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 77),(407, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 78),(408, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 79),(409, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 80),(410, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 81),(411, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 82),(412, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 83),(413, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 84),(414, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 85),(415, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 86),(416, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 87),(417, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 88),(418, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 89),(419, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 90),(420, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 91),(421, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 92),(422, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 93),(423, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 94),(424, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 95),(425, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 96),(426, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 97),(427, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 98),(428, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 99),(429, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 100),(430, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 101),(431, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 102),(432, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 103),(433, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 104),(434, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 105),(435, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 106),(436, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 107),(437, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 108),(438, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 109),(439, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 110),(440, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 111),(441, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 112),(442, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 113),(443, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 114),(444, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 115),(445, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 116),(446, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 117),(447, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 118),(448, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 119),(449, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 120),(450, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 121),(451, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 122),(452, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 123),(453, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 124),(454, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 125),(455, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 126),(456, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 127),(457, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 128),(458, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 129),(459, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 130),(460, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 131),(461, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 132),(462, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 133),(463, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 134),(464, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 135),(465, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 136),(466, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 137),(467, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 138),(468, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 139),(469, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 140),(470, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 141),(471, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 142),(472, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 143),(473, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 144),(474, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 145),(475, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 146),(476, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 147),(477, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 148),(478, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 149),(479, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 150),(480, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 151),(481, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 152),(482, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 153),(483, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 154),(484, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 155),(485, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 156),(486, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 157),(487, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 158),(488, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 159),(489, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 160),(490, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 161),(491, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 162),(492, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 163),(493, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 164),(494, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 165),(495, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 166),(496, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 167),(497, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 168),(498, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 169),(499, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 170),(500, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 171),(501, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 172),(502, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 173),(503, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 174),(504, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 175),(505, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 176),(506, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 177),(507, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 178),(508, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 179),(509, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 180),(510, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 181),(511, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 182),(512, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 183),(513, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 184),(514, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 185),(515, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 186),(516, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 187),(517, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 188),(518, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 189),(519, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 190),(520, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 191),(521, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 192),(522, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 193),(523, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 194),(524, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 195),(525, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 196),(526, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 197),(527, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 198),(528, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 199),(529, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 200),(530, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 201),(531, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 202),(532, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 203),(533, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 204),(534, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 205),(535, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 206),(536, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 207),(537, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 208),(538, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 209),(539, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 210),(540, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 211),(541, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 212),(542, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 213),(543, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 214),(544, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 215),(545, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 216),(546, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 217),(547, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 218),(548, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 219),(549, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 220),(550, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 221),(551, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 222),(552, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 223),(553, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 224),(554, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 225),(555, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 226),(556, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 227),(557, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 228),(558, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 229),(559, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 230),(560, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 231),(561, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 232),(562, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 233),(563, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 234),(564, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 235),(565, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 236),(566, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 237),(567, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 238),(568, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 239),(569, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 240),(570, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 241),(571, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 242),(572, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 243),(573, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 244),(574, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 245),(575, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 246),(576, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 247),(577, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 248),(578, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 249),(579, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 250),(580, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 251),(581, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 252),(582, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 253),(583, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 254),(584, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 255),(585, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 256),(586, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 257),(587, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 258),(588, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 259),(589, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 260),(590, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 261),(591, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 262),(592, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 263),(593, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 264),(594, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 265),(595, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 266),(596, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 267),(597, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 268),(598, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 269),(599, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 270),(600, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 271),(601, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 272),(602, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 273),(603, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 274),(604, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 275),(605, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 276),(606, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 277),(607, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 278),(608, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 279),(609, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 280),(610, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 281),(611, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 282),(612, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 283),(613, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 284),(614, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 285),(615, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 286),(616, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 287),(617, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 288),(618, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 289),(619, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 290),(620, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 291),(621, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 292),(622, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 293),(623, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 294),(624, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 295),(625, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 296),(626, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 297),(627, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 298),(628, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 299),(629, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 300),(630, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 301),(631, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 302),(632, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 303),(633, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 304),(634, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 305),(635, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 306),(636, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 307),(637, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 308),(638, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 309),(639, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 310),(640, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 311),(641, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 312),(642, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 313),(643, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 314),(644, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 315),(645, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 316),(646, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 317),(647, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 318),(648, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 319),(649, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 320),(650, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 321),(651, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 322),(652, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 323),(653, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 324),(654, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 325),(655, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 326),(656, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 327),(657, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 328),(658, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 329),(659, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 330),(660, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 331),(661, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 628),(662, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 629),(663, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 630),(664, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 631),(665, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 632),(666, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 633),(667, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 634),(668, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 635),(669, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 636),(670, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 637),(671, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 638),(672, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 639),(673, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 640),(674, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 641),(675, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 642),(676, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 643),(677, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 644),(678, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 645),(679, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 646),(680, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 647),(681, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 648),(682, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 649),(683, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 650),(684, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 651),(685, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 652),(686, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 653),(687, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 654),(688, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 370),(689, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 371),(690, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 656),(691, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 657),(692, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 663),(693, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 664),(694, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 665),(695, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 666),(696, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 667),(697, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 668),(698, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 669),(699, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 670),(700, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 671),(701, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 672),(702, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 673),(703, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 674),(704, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 675),(705, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 676),(706, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 677),(707, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 678),(708, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 679),(709, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 680),(710, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 681),(711, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 682),(712, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 683),(713, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 684),(714, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 685),(715, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 686),(716, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 687),(717, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 688),(718, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 730),(719, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 731),(720, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 732),(721, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 733),(722, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 734),(723, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 735),(724, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 736),(725, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 737),(726, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 738),(727, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 739),(728, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 740),(729, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 741),(730, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 742),(731, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 743),(732, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 744),(733, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 745),(734, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 746),(735, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 747),(736, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 748),(737, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 749),(738, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 750),(739, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 751),(740, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 752),(741, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 753),(742, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 754),(743, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 755),(744, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 756),(745, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 757),(746, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 758),(747, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 759),(748, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 760),(749, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 347),(750, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 348),(751, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 349),(752, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 350),(753, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 351),(754, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 352),(755, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 353),(756, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 354),(757, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 355),(758, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 356),(759, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 357),(760, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 358),(761, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 359),(762, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 360),(763, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 361),(764, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 362),(765, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 363),(766, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 364),(767, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 365),(768, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 366),(769, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 367),(770, '2010-10-17 22:17:29', NULL, NULL, 1, '', 1, NULL, 1, 368),(771, '2010-10-17 22:21:26', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 771),(772, '2010-10-17 22:22:23', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 772),(773, '2010-10-17 22:28:36', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 773),(774, '2010-10-17 22:29:27', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 774),(775, '2010-10-17 22:30:23', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 775),(776, '2010-10-17 22:31:41', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 776),(777, '2010-10-17 22:32:16', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 777),(778, '2010-10-17 22:32:57', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 778),(779, '2010-10-17 22:38:25', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 779),(780, '2010-10-17 22:41:43', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 780),(781, '2010-10-17 22:42:13', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 781),(782, '2010-10-17 22:42:43', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 782),(783, '2010-10-17 22:43:11', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 783),(784, '2010-10-17 22:45:02', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 784),(785, '2010-10-17 22:45:25', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 785),(786, '2010-10-17 22:46:01', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 786),(787, '2010-10-17 22:46:26', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 787),(788, '2010-10-17 22:46:52', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 788),(789, '2010-10-17 22:47:19', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 789),(790, '2010-10-17 22:47:45', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 790),(791, '2010-10-17 22:48:20', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 791),(792, '2010-10-17 22:48:55', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 792),(793, '2010-10-17 22:49:20', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 793),(794, '2010-10-17 22:51:14', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 794),(795, '2010-10-17 22:51:36', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 795),(796, '2010-10-17 22:52:10', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 796),(797, '2010-10-17 22:52:45', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 797),(798, '2010-10-17 22:53:08', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 798),(799, '2010-10-17 22:53:32', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 799),(800, '2010-10-17 22:54:10', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 800),(801, '2010-10-17 22:54:37', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 801),(802, '2010-10-17 22:55:08', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 802),(803, '2010-10-17 22:55:32', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 803),(804, '2010-10-17 22:55:56', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 804),(805, '2010-10-17 22:56:21', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 805),(806, '2010-10-17 22:57:12', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 806),(807, '2010-10-17 22:57:36', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 807),(808, '2010-10-17 22:58:00', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 808),(809, '2010-10-17 22:58:33', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 809),(810, '2010-10-17 22:58:58', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 810),(811, '2010-10-17 22:59:25', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 811),(812, '2010-10-17 22:59:44', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 812),(813, '2010-10-17 23:00:06', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 813),(814, '2010-10-17 23:00:27', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 814),(815, '2010-10-17 23:00:49', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 815),(816, '2010-10-17 23:01:09', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 816),(817, '2010-10-17 23:02:23', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 817),(818, '2010-10-17 23:02:44', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 818),(819, '2010-10-17 23:03:10', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 819),(820, '2010-10-17 23:03:30', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 820),(821, '2010-10-17 23:03:53', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 821),(822, '2010-10-17 23:04:19', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 822),(823, '2010-10-17 23:04:40', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 823),(824, '2010-10-17 23:05:02', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 824),(825, '2010-10-17 23:05:22', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 825),(826, '2010-10-17 23:05:47', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 826),(827, '2010-10-17 23:06:05', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 827),(828, '2010-10-17 23:06:31', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 828),(829, '2010-10-17 23:06:52', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 829),(830, '2010-10-17 23:07:13', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 830),(831, '2010-10-17 23:07:38', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 831),(832, '2010-10-17 23:07:58', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 832),(833, '2010-10-17 23:08:29', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 833),(834, '2010-10-17 23:08:50', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 834),(835, '2010-10-17 23:09:11', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 835),(836, '2010-10-17 23:09:38', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 836),(837, '2010-10-17 23:10:03', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 837),(838, '2010-10-17 23:10:25', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 838),(839, '2010-10-17 23:10:48', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 839),(840, '2010-10-17 23:11:14', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 840),(841, '2010-10-17 23:11:33', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 841),(842, '2010-10-17 23:11:54', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 842),(843, '2010-10-17 23:12:12', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 843),(844, '2010-10-17 23:12:32', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 844),(845, '2010-10-17 23:12:50', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 845),(846, '2010-10-17 23:13:11', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 846),(847, '2010-10-17 23:13:30', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 847),(848, '2010-10-17 23:13:48', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 848),(849, '2010-10-17 23:14:23', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 849),(850, '2010-10-17 23:14:45', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 850),(851, '2010-10-17 23:15:04', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 851),(852, '2010-10-17 23:15:22', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 852),(853, '2010-10-17 23:15:48', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 853),(854, '2010-10-17 23:16:10', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 854),(855, '2010-10-17 23:16:29', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 855),(856, '2010-10-17 23:16:48', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 856),(857, '2010-10-17 23:17:05', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 857),(858, '2010-10-17 23:17:28', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 858),(859, '2010-10-17 23:17:48', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 859),(860, '2010-10-17 23:18:10', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 860),(861, '2010-10-17 23:18:32', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 861),(862, '2010-10-17 23:18:50', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 862),(863, '2010-10-17 23:19:08', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 863),(864, '2010-10-17 23:19:33', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 864),(865, '2010-10-17 23:19:54', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 865),(866, '2010-10-17 23:20:13', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 866),(867, '2010-10-17 23:22:23', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 867),(868, '2010-10-17 23:22:42', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 868),(869, '2010-10-17 23:23:00', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 869),(870, '2010-10-17 23:23:19', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 870),(871, '2010-10-17 23:23:38', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 871),(872, '2010-10-17 23:23:59', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 872),(873, '2010-10-17 23:24:18', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 873),(874, '2010-10-17 23:24:40', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 874),(875, '2010-10-17 23:24:57', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 875),(876, '2010-10-17 23:25:53', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 876),(877, '2010-10-17 23:26:15', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 877),(878, '2010-10-17 23:26:42', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 878),(879, '2010-10-17 23:27:16', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 879),(880, '2010-10-17 23:27:44', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 880),(881, '2010-10-17 23:28:15', NULL, NULL, 1, '82.235.90.160', 2, NULL, 1, 881),(882, '2010-10-17 23:30:39', NULL, NULL, 1, '82.235.90.160', 3, 'WebRadio => WebTV', 1, 877),(883, '2010-10-17 23:30:59', NULL, NULL, 1, '82.235.90.160', 3, 'WebRadio => WebTV', 1, 878),(884, '2010-10-17 23:31:06', NULL, NULL, 1, '82.235.90.160', 3, 'WebRadio => WebTV', 1, 879),(885, '2010-10-17 23:33:31', NULL, NULL, 1, '82.235.90.160', 4, 'Submitted => Working', 1, 769),(886, '2010-10-17 23:33:59', NULL, NULL, 1, '82.235.90.160', 4, 'Submitted => Working', 1, 770),(887, '2010-10-17 23:34:28', NULL, NULL, 1, '82.235.90.160', 4, 'Submitted => Working', 1, 876),(888, '2010-10-17 23:34:59', NULL, NULL, 1, '82.235.90.160', 4, 'Submitted => Working', 1, 771),(889, '2010-10-17 23:35:35', NULL, NULL, 1, '82.235.90.160', 4, 'Submitted => Working', 1, 877),(890, '2010-10-17 23:36:04', NULL, NULL, 1, '82.235.90.160', 4, 'Submitted => Working', 1, 878),(891, '2010-10-17 23:36:29', NULL, NULL, 1, '82.235.90.160', 4, 'Submitted => Working', 1, 879),(892, '2010-10-17 23:36:52', NULL, NULL, 1, '82.235.90.160', 4, 'Submitted => Working', 1, 772),(893, '2010-10-17 23:37:16', NULL, NULL, 1, '82.235.90.160', 4, 'Submitted => Working', 1, 880),(894, '2010-10-17 23:37:33', NULL, NULL, 1, '82.235.90.160', 4, 'Submitted => Working', 1, 881),(895, '2010-10-20 23:27:28', NULL, NULL, 1, '82.235.90.160', 4, 'Submitted => Dead', 1, 773),(896, '2010-10-20 23:28:49', NULL, NULL, 1, '82.235.90.160', 4, 'Submitted => Working', 1, 774),(897, '2010-10-20 23:30:34', NULL, NULL, 1, '82.235.90.160', 4, 'Submitted => Working', 1, 775),(898, '2010-10-20 23:33:13', NULL, NULL, 1, '82.235.90.160', 4, 'Submitted => Dead', 1, 776),(899, '2010-10-20 23:33:24', NULL, NULL, 1, '82.235.90.160', 4, 'Submitted => Dead', 1, 777),(900, '2010-10-20 23:33:35', NULL, NULL, 1, '82.235.90.160', 4, 'Submitted => Dead', 1, 778),(901, '2010-10-20 23:33:56', NULL, NULL, 1, '82.235.90.160', 4, 'Submitted => Dead', 1, 779),(902, '2010-10-20 23:34:16', NULL, NULL, 1, '82.235.90.160', 4, 'Submitted => Dead', 1, 780),(903, '2010-10-20 23:34:49', NULL, NULL, 1, '82.235.90.160', 4, 'Submitted => Dead', 1, 781),(904, '2010-10-20 23:36:49', NULL, NULL, 1, '82.235.90.160', 4, 'Submitted => Working', 1, 782),(905, '2010-10-20 23:38:51', NULL, NULL, 1, '82.235.90.160', 4, 'Submitted => Dead', 1, 783),(906, '2010-10-20 23:39:32', NULL, NULL, 1, '82.235.90.160', 4, 'Submitted => Working', 1, 784),(907, '2010-10-20 23:39:53', NULL, NULL, 1, '82.235.90.160', 4, 'Submitted => Working', 1, 789),(908, '2010-10-20 23:42:17', NULL, NULL, 1, '82.235.90.160', 4, 'Submitted => Working', 1, 786),(909, '2010-10-20 23:43:47', NULL, NULL, 1, '82.235.90.160', 4, 'Submitted => Dead', 1, 787),(910, '2010-10-20 23:44:02', NULL, NULL, 1, '82.235.90.160', 4, 'Submitted => Dead', 1, 788),(911, '2010-10-27 04:37:22', NULL, NULL, NULL, '68.173.59.209', 2, NULL, 1, 882),(912, '2010-10-31 12:03:44', NULL, NULL, 1, '78.228.4.15', 3, 'WebTV => WebLink, none => ', 1, 882),(913, '2010-11-06 17:26:32', NULL, NULL, NULL, '86.73.4.93', 2, NULL, 1, 883),(914, '2010-11-06 17:28:49', NULL, NULL, NULL, '86.73.4.93', 2, NULL, 1, 884),(915, '2010-11-08 19:54:35', NULL, NULL, 1, '78.250.197.65', 4, 'Submitted => Working', 1, 884),(916, '2010-11-14 20:48:10', NULL, NULL, 1, '78.250.200.243', 2, NULL, 1, 885),(917, '2010-11-14 20:48:18', NULL, NULL, 1, '78.250.200.243', 4, 'Submitted => Working', 1, 885),(918, '2010-11-21 13:22:47', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 883),(919, '2010-11-21 13:24:49', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 785),(920, '2010-11-21 13:25:02', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 790),(921, '2010-11-21 13:25:20', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 791),(922, '2010-11-21 13:25:26', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 793),(923, '2010-11-21 13:25:34', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 792),(924, '2010-11-21 13:26:10', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 794),(925, '2010-11-21 13:26:27', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 795),(926, '2010-11-21 13:27:00', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 800),(927, '2010-11-21 13:27:20', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 796),(928, '2010-11-21 13:28:52', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 797),(929, '2010-11-21 13:28:59', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 798),(930, '2010-11-21 13:29:05', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 799),(931, '2010-11-21 13:29:38', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 801),(932, '2010-11-21 14:22:45', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 802),(933, '2010-11-21 14:23:01', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 803),(934, '2010-11-21 14:23:23', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 804),(935, '2010-11-21 14:23:57', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 805),(936, '2010-11-21 14:24:02', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 806),(937, '2010-11-21 14:24:09', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 807),(938, '2010-11-21 14:24:51', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 810),(939, '2010-11-21 14:24:57', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 809),(940, '2010-11-21 14:25:07', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 808),(941, '2010-11-21 14:25:45', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 811),(942, '2010-11-21 14:25:57', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 812),(943, '2010-11-21 14:26:06', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 813),(944, '2010-11-21 14:26:17', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 814),(945, '2010-11-21 14:26:35', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 815),(946, '2010-11-21 14:27:12', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 816),(947, '2010-11-21 14:27:28', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 817),(948, '2010-11-21 14:28:17', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 818),(949, '2010-11-21 14:28:39', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 819),(950, '2010-11-21 14:29:25', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 820),(951, '2010-11-21 14:29:38', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 821),(952, '2010-11-21 14:29:52', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 822),(953, '2010-11-21 14:30:02', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 823),(954, '2010-11-21 14:30:27', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 824),(955, '2010-11-21 14:31:54', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 841),(956, '2010-11-21 14:32:26', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 842),(957, '2010-11-21 14:32:34', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 843),(958, '2010-11-21 14:32:46', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 844),(959, '2010-11-21 14:33:00', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 845),(960, '2010-11-21 14:33:16', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 846),(961, '2010-11-21 14:33:28', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 847),(962, '2010-11-21 14:36:04', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 829),(963, '2010-11-21 14:36:19', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 830),(964, '2010-11-21 14:36:41', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 831),(965, '2010-11-21 14:37:02', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 832),(966, '2010-11-21 14:37:37', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 833),(967, '2010-11-21 14:38:07', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 834),(968, '2010-11-21 14:38:13', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 835),(969, '2010-11-21 14:39:55', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 839),(970, '2010-11-21 14:40:10', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 848),(971, '2010-11-21 14:40:33', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 849),(972, '2010-11-21 14:40:46', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 850),(973, '2010-11-21 14:41:19', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 851),(974, '2010-11-21 14:41:41', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 852),(975, '2010-11-21 14:41:57', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 853),(976, '2010-11-21 14:42:14', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 854),(977, '2010-11-21 14:42:32', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 855),(978, '2010-11-21 14:42:47', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 856),(979, '2010-11-21 14:43:46', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 858),(980, '2010-11-21 14:44:06', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 857),(981, '2010-11-21 14:45:07', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 859),(982, '2010-11-21 14:45:22', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 866),(983, '2010-11-21 14:45:55', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 864),(984, '2010-11-21 14:46:24', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 865),(985, '2010-11-21 14:46:42', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 862),(986, '2010-11-21 14:47:00', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 863),(987, '2010-11-21 14:48:25', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 867),(988, '2010-11-21 14:48:57', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 868),(989, '2010-11-21 14:49:20', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 869),(990, '2010-11-21 14:50:06', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 870),(991, '2010-11-21 14:50:15', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 871),(992, '2010-11-21 14:50:24', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 872),(993, '2010-11-21 14:51:05', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 873),(994, '2010-11-21 14:51:18', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 874),(995, '2010-11-21 14:52:08', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 875),(996, '2010-11-21 14:52:55', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 860),(997, '2010-11-21 14:53:01', NULL, NULL, 1, '78.250.221.73', 4, 'Submitted => Working', 1, 861),(998, '2010-11-25 01:52:45', NULL, NULL, NULL, '218.71.136.44', 2, NULL, 1, 886),(999, '2010-11-25 01:56:33', NULL, NULL, NULL, '218.71.136.44', 2, NULL, 1, 887),(1000, '2010-11-25 01:58:54', NULL, NULL, NULL, '218.71.136.44', 2, NULL, 1, 888),(1001, '2010-11-25 02:00:20', NULL, NULL, NULL, '218.71.136.44', 2, NULL, 1, 889),(1002, '2010-11-25 02:02:19', NULL, NULL, NULL, '218.71.136.44', 2, NULL, 1, 890),(1003, '2010-11-25 02:06:34', NULL, NULL, NULL, '218.71.136.44', 2, NULL, 1, 891),(1004, '2010-11-26 19:29:41', NULL, NULL, 1, '78.250.211.77', 3, '??????????? => CCTV-1', 1, 886),(1005, '2010-11-26 19:29:54', NULL, NULL, 1, '78.250.211.77', 3, '??????????? => CCTV-1', 1, 887),(1006, '2010-11-26 19:30:03', NULL, NULL, 1, '78.250.211.77', 3, 'CCTV-1 => CCTV-2', 1, 887),(1007, '2010-11-26 19:30:36', NULL, NULL, 1, '78.250.211.77', 3, '???????????cctv3 => CCTV-3', 1, 888),(1008, '2010-11-26 19:30:52', NULL, NULL, 1, '78.250.211.77', 3, '???????????CCTV4 => CCTV-4', 1, 889),(1009, '2010-11-26 19:31:13', NULL, NULL, 1, '78.250.211.77', 3, '??????????CCTV5 => CCTV-5', 1, 890),(1010, '2010-11-26 19:37:44', NULL, NULL, 1, '78.250.211.77', 3, '?????IFENG1 => 凤凰中文 IFENG1', 1, 891),(1011, '2010-11-26 23:37:47', NULL, NULL, 1, '78.250.211.77', 2, NULL, 1, 892),(1012, '2010-11-26 23:42:13', NULL, NULL, 1, '78.250.211.77', 2, NULL, 1, 893),(1013, '2010-11-27 00:03:59', NULL, NULL, 1, '78.250.211.77', 2, NULL, 1, 894),(1014, '2010-11-28 23:33:21', NULL, NULL, 1, '78.250.213.7', 4, 'Submitted => Working', 1, 886),(1015, '2010-11-28 23:33:53', NULL, NULL, 1, '78.250.213.7', 4, 'Submitted => Working', 1, 891),(1016, '2010-11-28 23:34:03', NULL, NULL, 1, '78.250.213.7', 4, 'Submitted => Working', 1, 887),(1017, '2010-11-28 23:34:15', NULL, NULL, 1, '78.250.213.7', 4, 'Submitted => Working', 1, 888),(1018, '2010-11-28 23:34:20', NULL, NULL, 1, '78.250.213.7', 4, 'Submitted => Working', 1, 889),(1019, '2010-11-28 23:34:30', NULL, NULL, 1, '78.250.213.7', 4, 'Submitted => Working', 1, 890),(1020, '2010-11-28 23:34:59', NULL, NULL, 1, '78.250.213.7', 4, 'Submitted => Working', 1, 893),(1021, '2010-11-28 23:35:06', NULL, NULL, 1, '78.250.213.7', 4, 'Submitted => Working', 1, 894);

UNLOCK TABLES;

--
-- Table structure for table `wtvmT_Lang`
--



CREATE TABLE `wtvmT_Lang` (
  `Code` char(2) NOT NULL,
  `Label` varchar(20) NOT NULL,
  PRIMARY KEY (`Code`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


--
-- Dumping data for table `wtvmT_Lang`
--

LOCK TABLES `wtvmT_Lang` WRITE;

INSERT INTO `wtvmT_Lang` VALUES ('aa','Afar'),('ab','Abkhazian'),('af','Afrikaans'),('am','Amharic'),('ar','Arabic'),('as','Assamese'),('ay','Aymara'),('az','Azerbaijani'),('ba','Bashkir'),('be','Byelorussian'),('bg','Bulgarian'),('bh','Bihari'),('bi','Bislama'),('bn','Bengali/Bangla'),('bo','Tibetan'),('br','Breton'),('ca','Catalan'),('co','Corsican'),('cs','Czech'),('cy','Welsh'),('da','Danish'),('de','German'),('dz','Bhutani'),('el','Greek'),('en','English'),('eo','Esperanto'),('es','Spanish'),('et','Estonian'),('eu','Basque'),('fa','Persian'),('fi','Finnish'),('fj','Fiji'),('fo','Faeroese'),('fr','French'),('fy','Frisian'),('ga','Irish'),('gd','Scots/Gaelic'),('gl','Galician'),('gn','Guarani'),('gu','Gujarati'),('ha','Hausa'),('hi','Hindi'),('hr','Croatian'),('hu','Hungarian'),('hy','Armenian'),('ia','Interlingua'),('ie','Interlingue'),('ik','Inupiak'),('in','Indonesian'),('is','Icelandic'),('it','Italian'),('iw','Hebrew'),('ja','Japanese'),('ji','Yiddish'),('jw','Javanese'),('ka','Georgian'),('kk','Kazakh'),('kl','Greenlandic'),('km','Cambodian'),('kn','Kannada'),('ko','Korean'),('ks','Kashmiri'),('ku','Kurdish'),('ky','Kirghiz'),('la','Latin'),('ln','Lingala'),('lo','Laothian'),('lt','Lithuanian'),('lv','Latvian/Lettish'),('mg','Malagasy'),('mi','Maori'),('mk','Macedonian'),('ml','Malayalam'),('mn','Mongolian'),('mo','Moldavian'),('mr','Marathi'),('ms','Malay'),('mt','Maltese'),('my','Burmese'),('na','Nauru'),('ne','Nepali'),('nl','Dutch'),('no','Norwegian'),('oc','Occitan'),('om','(Afan)/Oromoor/Oriya'),('pa','Punjabi'),('pl','Polish'),('ps','Pashto/Pushto'),('pt','Portuguese'),('qu','Quechua'),('rm','Rhaeto-Romance'),('rn','Kirundi'),('ro','Romanian'),('ru','Russian'),('rw','Kinyarwanda'),('sa','Sanskrit'),('sd','Sindhi'),('sg','Sangro'),('sh','Serbo-Croatian'),('si','Singhalese'),('sk','Slovak'),('sl','Slovenian'),('sm','Samoan'),('sn','Shona'),('so','Somali'),('sq','Albanian'),('sr','Serbian'),('ss','Siswati'),('st','Sesotho'),('su','Sundanese'),('sv','Swedish'),('sw','Swahili'),('ta','Tamil'),('te','Tegulu'),('tg','Tajik'),('th','Thai'),('ti','Tigrinya'),('tk','Turkmen'),('tl','Tagalog'),('tn','Setswana'),('to','Tonga'),('tr','Turkish'),('ts','Tsonga'),('tt','Tatar'),('tw','Twi'),('uk','Ukrainian'),('ur','Urdu'),('uz','Uzbek'),('vi','Vietnamese'),('vo','Volapuk'),('wo','Wolof'),('xh','Xhosa'),('yo','Yoruba'),('zh','Chinese'),('zu','Zulu');
UNLOCK TABLES;

--
-- Table structure for table `wtvmT_StreamStatus`
--



CREATE TABLE `wtvmT_StreamStatus` (
  `Code` int(11) NOT NULL,
  `Label` varchar(20) NOT NULL,
  `Color` varchar(10) NOT NULL,
  `Searchable` tinyint(1) NOT NULL DEFAULT '1',
  PRIMARY KEY (`Code`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


--
-- Dumping data for table `wtvmT_StreamStatus`
--

LOCK TABLES `wtvmT_StreamStatus` WRITE;

INSERT INTO `wtvmT_StreamStatus` VALUES (1,'Submitted','black',1),(2,'Working','green',1),(3,'Deprecated','orangered',1),(4,'Duplicate','orange',1),(5,'Invalid','gray',1),(6,'Dead','darkred',1),(7,'Forbidden','red',0);

UNLOCK TABLES;

--
-- Table structure for table `wtvmT_TVChannel`
--



CREATE TABLE wtvmT_TVChannel (
	Id INTEGER NOT NULL AUTO_INCREMENT,
	Name VARCHAR(50) NOT NULL,
	LangCode CHAR(2) NULL,
	CountryCode CHAR(2) NULL,
	ChannelTypeId INTEGER NULL,
	Logo VARCHAR(20) NULL,
	WebSite VARCHAR(255) NULL,
	GuideUrl VARCHAR(255) NULL,
	CONSTRAINT PK_TVChannel PRIMARY KEY(Id),
	CONSTRAINT FK_TVChannel_Lang FOREIGN KEY (LangCode)
		REFERENCES wtvmT_Lang (Code) ON DELETE SET NULL,
	CONSTRAINT FK_TVChannel_Country FOREIGN KEY (CountryCode)
		REFERENCES wtvmT_Country(Code) ON DELETE SET NULL,
	CONSTRAINT FK_TVChannel_TVChannelType FOREIGN KEY (ChannelTypeId)
		REFERENCES wtvmT_TVChannelType(Id) ON DELETE SET NULL
) CHARSET=utf8;


--
-- Table structure for table `wtvmT_TVChannelType`
--



CREATE TABLE `wtvmT_TVChannelType` (
  `Id` int(11) NOT NULL AUTO_INCREMENT,
  `Label` varchar(20) NOT NULL,
  PRIMARY KEY (`Id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Table structure for table `wtvmT_WebStream`
--



CREATE TABLE `wtvmT_WebStream` (
  `Id` INTEGER NOT NULL AUTO_INCREMENT,
  `Name` varchar(100) DEFAULT NULL,
  `Url` varchar(255) DEFAULT NULL,
  `TypeStream` int(11) DEFAULT NULL,
  `StreamStatusCode` int(11) NOT NULL DEFAULT '1',
  `RequiredIsp` varchar(50) DEFAULT NULL,
  `LangCode` char(2) DEFAULT NULL,
  `CountryCode` char(2) DEFAULT NULL,
  `Comments` varchar(255) DEFAULT NULL,
  `SubmissionDate` datetime NOT NULL,
  `TvChannelId` int(11) DEFAULT NULL,
  PRIMARY KEY (`Id`),
  KEY `FK_WebStream_StreamStatus` (`StreamStatusCode`),
  KEY `FK_WebStream_Lang` (`LangCode`),
  KEY `FK_WebStream_TVChannel` (`TvChannelId`),
  KEY `FK_WebStream_Country` (`CountryCode`),
  CONSTRAINT `FK_WebStream_Country` FOREIGN KEY (`CountryCode`) REFERENCES `wtvmT_Country` (`Code`),
  CONSTRAINT `FK_WebStream_Lang` FOREIGN KEY (`LangCode`) REFERENCES `wtvmT_Lang` (`Code`) ON DELETE SET NULL,
  CONSTRAINT `FK_WebStream_StreamStatus` FOREIGN KEY (`StreamStatusCode`) REFERENCES `wtvmT_StreamStatus` (`Code`),
  CONSTRAINT `FK_WebStream_TVChannel` FOREIGN KEY (`TvChannelId`) REFERENCES `wtvmT_TVChannel` (`Id`) ON DELETE SET NULL
) CHARSET=utf8;

--
-- Dumping data for table `wtvmT_WebStream`
--

LOCK TABLES `wtvmT_WebStream` WRITE;


UNLOCK TABLES;


--
-- Table structure for table `wtvmT_Action`
--



CREATE TABLE `wtvmT_Action` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `title` varchar(255) NOT NULL,
  `comment` text,
  `subject` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8;

--
-- Table structure for table `wtvmT_Activities`
--



CREATE TABLE `wtvmT_Activities` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` int(11) NOT NULL,
  `user_id` int(11) DEFAULT NULL,
  `remote_addr` varchar(16) DEFAULT NULL,
  `http_user_agent` varchar(255) DEFAULT NULL,
  `action` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8;

--
-- Table structure for table `wtvmT_Friendship`
--



CREATE TABLE `wtvmT_Friendship` (
  `inviter_id` int(11) NOT NULL,
  `friend_id` int(11) NOT NULL,
  `status` int(11) NOT NULL,
  `acknowledgetime` int(11) DEFAULT NULL,
  `requesttime` int(11) DEFAULT NULL,
  `updatetime` int(11) DEFAULT NULL,
  `message` varchar(255) NOT NULL,
  PRIMARY KEY (`inviter_id`,`friend_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Table structure for table `wtvmT_Membership`
--



CREATE TABLE `wtvmT_Membership` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `Membership_id` int(11) NOT NULL,
  `user_id` int(11) NOT NULL,
  `payment_id` int(11) NOT NULL,
  `order_date` int(11) NOT NULL,
  `end_date` int(11) DEFAULT NULL,
  `payment_date` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=10000 DEFAULT CHARSET=utf8;

--
-- Table structure for table `wtvmT_Messages`
--



CREATE TABLE `wtvmT_Messages` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `timestamp` int(10) unsigned NOT NULL,
  `from_user_id` int(10) unsigned NOT NULL,
  `to_user_id` int(10) unsigned NOT NULL,
  `title` varchar(45) NOT NULL,
  `message` text,
  `message_read` tinyint(1) NOT NULL,
  `answered` tinyint(1) NOT NULL,
  `draft` tinyint(1) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `fk_messages_users` (`from_user_id`),
  KEY `fk_messages_users1` (`to_user_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Table structure for table `wtvmT_Payment`
--



CREATE TABLE `wtvmT_Payment` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `title` varchar(255) NOT NULL,
  `text` text,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;


--
-- Dumping data for table `wtvmT_Payment`
--

LOCK TABLES `wtvmT_Payment` WRITE;

INSERT INTO `wtvmT_Payment` VALUES (1,'Prepayment',NULL),(2,'Paypal',NULL);

UNLOCK TABLES;

--
-- Table structure for table `wtvmT_Permission`
--



CREATE TABLE `wtvmT_Permission` (
  `principal_id` int(11) NOT NULL,
  `subordinate_id` int(11) NOT NULL DEFAULT '0',
  `type` enum('user','role') NOT NULL,
  `action` int(11) NOT NULL,
  `template` tinyint(1) NOT NULL,
  `comment` text,
  PRIMARY KEY (`principal_id`,`subordinate_id`,`type`,`action`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


--
-- Dumping data for table `wtvmT_Permission`
--

LOCK TABLES `wtvmT_Permission` WRITE;

INSERT INTO `wtvmT_Permission` VALUES (3,0,'role',1,0,'Users can write messagse'),(3,0,'role',2,0,'Users can receive messagse'),(3,0,'role',3,0,'Users are able to view visits of his profile');

UNLOCK TABLES;

--
-- Table structure for table `wtvmT_PrivacySetting`
--



CREATE TABLE `wtvmT_PrivacySetting` (
  `user_id` int(10) unsigned NOT NULL,
  `message_new_friendship` tinyint(1) NOT NULL,
  `message_new_message` tinyint(1) NOT NULL,
  `message_new_profilecomment` tinyint(1) NOT NULL,
  `appear_in_search` tinyint(1) NOT NULL DEFAULT '1',
  `ignore_users` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`user_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


--
-- Dumping data for table `wtvmT_PrivacySetting`
--

LOCK TABLES `wtvmT_PrivacySetting` WRITE;

INSERT INTO `wtvmT_PrivacySetting` VALUES (2,0,0,0,1,NULL);

UNLOCK TABLES;

--
-- Table structure for table `wtvmT_Profile_Comment`
--



CREATE TABLE `wtvmT_Profile_Comment` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `user_id` int(11) NOT NULL,
  `profile_id` int(11) NOT NULL,
  `comment` text NOT NULL,
  `createtime` int(11) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Table structure for table `wtvmT_Profile_Fields`
--



CREATE TABLE `wtvmT_Profile_Fields` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `field_group_id` int(10) unsigned NOT NULL DEFAULT '0',
  `varname` varchar(50) NOT NULL,
  `title` varchar(255) NOT NULL,
  `hint` text NOT NULL,
  `field_type` varchar(50) NOT NULL,
  `field_size` int(3) NOT NULL DEFAULT '0',
  `field_size_min` int(3) NOT NULL DEFAULT '0',
  `required` int(1) NOT NULL DEFAULT '0',
  `match` varchar(255) NOT NULL,
  `range` varchar(255) NOT NULL,
  `error_message` varchar(255) NOT NULL,
  `other_validator` varchar(255) NOT NULL,
  `default` varchar(255) NOT NULL,
  `position` int(3) NOT NULL DEFAULT '0',
  `visible` int(1) NOT NULL DEFAULT '0',
  `related_field_name` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `varname` (`varname`,`visible`)
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf8;


--
-- Dumping data for table `wtvmT_Profile_Fields`
--

LOCK TABLES `wtvmT_Profile_Fields` WRITE;

INSERT INTO `wtvmT_Profile_Fields` VALUES (1,0,'email','E-Mail','','VARCHAR',255,0,1,'','','','CEmailValidator','',0,2,NULL),(2,0,'firstname','First name','','VARCHAR',255,0,1,'','','','','',0,2,NULL),(3,0,'lastname','Last name','','VARCHAR',255,0,1,'','','','','',0,2,NULL),(4,0,'street','Street','','VARCHAR',255,0,0,'','','','','',0,1,NULL),(5,0,'city','City','','VARCHAR',255,0,0,'','','','','',0,1,NULL),(6,0,'about','About','','TEXT',255,0,0,'','','','','',0,1,NULL);

UNLOCK TABLES;

--
-- Table structure for table `wtvmT_Profile_Fields_Group`
--



CREATE TABLE `wtvmT_Profile_Fields_Group` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `group_name` varchar(50) NOT NULL,
  `title` varchar(255) NOT NULL,
  `position` int(3) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Table structure for table `wtvmT_Profile_Visit`
--



CREATE TABLE `wtvmT_Profile_Visit` (
  `visitor_id` int(11) NOT NULL,
  `visited_id` int(11) NOT NULL,
  `timestamp_first_visit` int(11) NOT NULL,
  `timestamp_last_visit` int(11) NOT NULL,
  `num_of_visits` int(11) NOT NULL,
  PRIMARY KEY (`visitor_id`,`visited_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Table structure for table `wtvmT_Profiles`
--



CREATE TABLE `wtvmT_Profiles` (
  `profile_id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `user_id` int(10) unsigned NOT NULL,
  `timestamp` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `privacy` enum('protected','private','public') NOT NULL,
  `lastname` varchar(50) NOT NULL DEFAULT '',
  `firstname` varchar(50) NOT NULL DEFAULT '',
  `show_friends` tinyint(1) DEFAULT '1',
  `allow_comments` tinyint(1) DEFAULT '1',
  `email` varchar(255) NOT NULL DEFAULT '',
  `street` varchar(255) DEFAULT NULL,
  `city` varchar(255) DEFAULT NULL,
  `about` text,
  PRIMARY KEY (`profile_id`),
  KEY `fk_profiles_users` (`user_id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;


--
-- Dumping data for table `wtvmT_Profiles`
--

LOCK TABLES `wtvmT_Profiles` WRITE;

INSERT INTO `wtvmT_Profiles` VALUES (1,1,'2011-01-14 22:38:43','protected','admin','admin',1,1,'webmaster@example.com',NULL,NULL,NULL),(2,2,'2011-01-14 22:38:43','protected','demo','demo',1,1,'demo@example.com',NULL,NULL,NULL);

UNLOCK TABLES;

--
-- Table structure for table `wtvmT_Roles`
--



CREATE TABLE `wtvmT_Roles` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `title` varchar(255) NOT NULL,
  `description` varchar(255) DEFAULT NULL,
  `selectable` tinyint(1) NOT NULL COMMENT 'Selectable on Registration?',
  `searchable` tinyint(1) NOT NULL COMMENT 'Can be searched',
  `autoassign` tinyint(1) NOT NULL COMMENT 'Autoassign on new users',
  `is_Membership_possible` tinyint(1) NOT NULL,
  `price` double DEFAULT NULL COMMENT 'Price (when using Membership module)',
  `duration` int(11) DEFAULT NULL COMMENT 'How long a Membership is valid',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8;


--
-- Dumping data for table `wtvmT_Roles`
--

LOCK TABLES `wtvmT_Roles` WRITE;

INSERT INTO `wtvmT_Roles` VALUES (1,'UserCreator','This users can create new Users',0,0,0,0,0,0),(2,'UserRemover','This users can remove other Users',0,0,0,0,0,0),(3,'Demo','Users having the demo role',0,1,0,0,0,0),(4,'Business','Example Business account',0,0,0,0,9.99,7),(5,'Premium','Example Premium account',0,0,0,0,19.99,28);

UNLOCK TABLES;

--
-- Table structure for table `wtvmT_User_Has_Role`
--



CREATE TABLE `wtvmT_User_Has_Role` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `user_id` int(10) unsigned NOT NULL,
  `role_id` int(10) unsigned NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;


--
-- Dumping data for table `wtvmT_User_Has_Role`
--

LOCK TABLES `wtvmT_User_Has_Role` WRITE;

INSERT INTO `wtvmT_User_Has_Role` VALUES (1,2,3);

UNLOCK TABLES;

--
-- Table structure for table `wtvmT_UserGroup`
--



CREATE TABLE `wtvmT_UserGroup` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `owner_id` int(11) NOT NULL,
  `title` varchar(255) NOT NULL,
  `description` text NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Table structure for table `wtvmT_YumSettings`
--



CREATE TABLE `wtvmT_YumSettings` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `title` varchar(255) NOT NULL,
  `is_active` tinyint(1) NOT NULL DEFAULT '0',
  `preserveProfiles` tinyint(1) NOT NULL DEFAULT '1',
  `enableAvatar` tinyint(1) NOT NULL DEFAULT '1',
  `registrationType` tinyint(1) NOT NULL DEFAULT '4',
  `enableRecovery` tinyint(1) NOT NULL DEFAULT '1',
  `enableProfileHistory` tinyint(1) NOT NULL DEFAULT '1',
  `messageSystem` enum('None','Plain','Dialog') NOT NULL DEFAULT 'Dialog',
  `notifyType` enum('None','Digest','Instant','User','Treshhold') NOT NULL DEFAULT 'User',
  `password_expiration_time` int(11) DEFAULT NULL,
  `readOnlyProfiles` tinyint(1) NOT NULL DEFAULT '0',
  `loginType` int(11) NOT NULL,
  `notifyemailchange` enum('oldemail','newemail') DEFAULT NULL,
  `enableCaptcha` tinyint(1) NOT NULL DEFAULT '1',
  `ldap_host` varchar(255) DEFAULT NULL,
  `ldap_port` int(5) DEFAULT NULL,
  `ldap_basedn` varchar(255) DEFAULT NULL,
  `ldap_protocol` enum('2','3') NOT NULL DEFAULT '3',
  `ldap_autocreate` tinyint(1) NOT NULL DEFAULT '1',
  `ldap_tls` tinyint(1) NOT NULL DEFAULT '0',
  `ldap_transfer_attr` tinyint(1) NOT NULL DEFAULT '1',
  `ldap_transfer_pw` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;


--
-- Dumping data for table `wtvmT_YumSettings`
--

LOCK TABLES `wtvmT_YumSettings` WRITE;

INSERT INTO `wtvmT_YumSettings` VALUES (1,'User',1,1,1,4,1,1,'Dialog','Instant',100,0,3,'newemail',1,'',NULL,'','3',1,0,1,0);

UNLOCK TABLES;

--
-- Table structure for table `wtvmT_Users`
--





--
-- Table structure for table `wtvmT_YumTextSettings`
--



CREATE TABLE `wtvmT_YumTextSettings` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `language` enum('en_us','de','fr','pl','ru','es') NOT NULL DEFAULT 'en_us',
  `text_registration_header` text,
  `text_registration_footer` text,
  `text_login_header` text,
  `text_login_footer` text,
  `text_email_registration` text,
  `subject_email_registration` text,
  `text_email_recovery` text,
  `text_email_activation` text,
  `text_friendship_new` text,
  `text_profilecomment_new` text,
  `text_message_new` text,
  `text_Membership_ordered` text,
  `text_payment_arrived` text,
  `text_Membership_header` text,
  `text_Membership_footer` text,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8;


--
-- Dumping data for table `wtvmT_YumTextSettings`
--

LOCK TABLES `wtvmT_YumTextSettings` WRITE;

INSERT INTO `wtvmT_YumTextSettings` VALUES (1,'en_us','Welcome at the registration System','When registering at this System, you automatically accept our terms.','Welcome!','','You have registered for this Application. To confirm your E-Mail address, please visit {activation_url}','You have registered for an application','You have requested a new Password. To set your new Password,\n										please go to {activation_url}','Your account has been activated. Thank you for your registration.','New friendship Request from {user_from}: {message}. To accept or ignore this request, go to your friendship page: {link_friends} or go to your profile: {link_profile}','You have a new profile comment from {user}: {message} visit your profile: {link}','You have received a new message from {user}: {message}','Your order of Membership {Membership} on {order_date} has been taken. Your order Number is {id}. You have choosen the payment style {payment}.','Your payment has been received on {payment_date} and your Membership {id} is now active','Please select a Membership of your choice','Your advantages: <br /> Premium: Everything is better <br /> Business: Everything is much better '),(2,'de','Willkommen zum System.','Mit der Anmeldung bestätigen Sie unsere allgemeinen Bedingungen.','Willkommen!','','Sie haben sich für unsere Applikation registriert. Bitte bestätigen Sie ihre E-Mail adresse mit diesem Link: {activation_url}','Sie haben sich für eine Applikation registriert.','Sie haben ein neues Passwort angefordert. Bitte klicken Sie diesen link: {activation_url}','Ihr Konto wurde freigeschaltet.','Der Benutzer {user} hat Ihnen eine Freundschaftsanfrage gesendet. \n\n							 Nachricht: {message}\n\n							 Klicken sie <a href=\"{link_friends}\">hier</a>, um diese Anfrage zu bestätigen oder zu ignorieren. Alternativ können sie <a href=\"{link_profile}\">hier</a> auf ihre Profilseite zugreifen.','\n							 Benutzer {username} hat Ihnen eine Nachricht auf Ihrer Pinnwand hinterlassen: \n\n							 {message}\n\n							 <a href=\"{link}\">hier</a> geht es direkt zu Ihrer Pinnwand!','Sie haben eine neue Nachricht von {user} bekommen: {message}','Ihre Bestellung der Mitgliedschaft {Membership} wurde am {order_date} entgegen genommen. Die gewählte Zahlungsart ist {payment}. Die Auftragsnummer lautet {id}.','Ihre Zahlung wurde am {payment_date} entgegen genommen. Ihre Mitgliedschaft mit der Nummer {id} ist nun Aktiv.','Bitte wählen Sie ein Paket ihrer Wahl:','Ihre Vorteile: '),(3,'es','Bienvenido al sistema de registro','Al registrarse en este sistema, usted está aceptando nuestros términos.','¡Bienvenido!','','Te has registrado en esta aplicación. Para confirmar tu dirección de correo electrónico, por favor, visita {activation_url}.','Te has registrado en esta aplicación.','Has solicitado una nueva contraseña. Para establecer una nueva contraseña, por favor ve a {activation_url}','Tu cuenta ha sido activada. Gracias por registrarte.','Has recibido una nueva solicitud de amistad de {user_from}: {message} Ve a tus contactos: {link}','Tienes un nuevo comentario en tu perfil de {user}: {message} visita tu perfil: {link}','Has recibido un mensaje de {user}: {message}','{id}','{payment_date}{id}','','');

UNLOCK TABLES;
