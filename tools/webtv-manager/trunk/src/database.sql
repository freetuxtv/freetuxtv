-- ALTER DATABASE freetuxtv CHARACTER SET utf8;

-- Version 0.1.0

CREATE TABLE IF NOT EXISTS wtvmT_Lang (
	Code CHAR(2) NOT NULL,
	Label VARCHAR(20) NOT NULL,
	CONSTRAINT PK_Lang PRIMARY KEY(Code)
) TYPE=InnoDB;

CREATE TABLE IF NOT EXISTS wtvmT_Country (
	Code CHAR(2) NOT NULL,
	Label VARCHAR(20) NOT NULL,
	CONSTRAINT PK_Country PRIMARY KEY(Code)
) TYPE=InnoDB;

CREATE TABLE IF NOT EXISTS wtvmT_TVChannelType (
	Id INTEGER NOT NULL AUTO_INCREMENT,
	Label VARCHAR(20) NOT NULL,
	CONSTRAINT PK_TVChannelType PRIMARY KEY(Id)
) TYPE=InnoDB;

CREATE TABLE IF NOT EXISTS wtvmT_TVChannel (
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
) TYPE=InnoDB;

CREATE TABLE IF NOT EXISTS wtvmT_StreamStatus (
	Code INTEGER NOT NULL,
	Label VARCHAR(20) NOT NULL,
	Color VARCHAR(10) NOT NULL,
	Searchable BOOL NOT NULL DEFAULT TRUE,
	CONSTRAINT PK_StreamStatus PRIMARY KEY(Code)
) TYPE=InnoDB;

CREATE TABLE IF NOT EXISTS wtvmT_WebStream (
	Id INTEGER NOT NULL AUTO_INCREMENT,
	Name VARCHAR(50) NOT NULL,
	Url VARCHAR(255) NULL,
	TypeStream INTEGER NULL,
	StreamStatusCode INTEGER NOT NULL DEFAULT '1',
	RequiredIsp VARCHAR(50) NULL,
	LangCode CHAR(2) NULL,
	Comments VARCHAR(255) NULL,
	SubmissionDate DATETIME NOT NULL,
	TvChannelId INTEGER NULL,
	CONSTRAINT PK_WebStream PRIMARY KEY(Id),
	CONSTRAINT FK_WebStream_StreamStatus FOREIGN KEY (StreamStatusCode)
		REFERENCES wtvmT_StreamStatus(Code),
	CONSTRAINT FK_WebStream_Lang FOREIGN KEY (LangCode)
		REFERENCES wtvmT_Lang (Code) ON DELETE SET NULL,
	CONSTRAINT FK_WebStream_TVChannel FOREIGN KEY (TvChannelId)
		REFERENCES wtvmT_TVChannel(Id) ON DELETE SET NULL
) TYPE=InnoDB;

CREATE TABLE IF NOT EXISTS wtvmT_User (
	Id INTEGER NOT NULL AUTO_INCREMENT,
	Username VARCHAR(20) NOT NULL,
	Password VARCHAR(64) NOT NULL,
	Email VARCHAR(255) NOT NULL,
	Rights INTEGER(10) NOT NULL DEFAULT '0',
	CONSTRAINT PK_User PRIMARY KEY(Id)
) TYPE=InnoDB;

INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('English', 'en');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Afar', 'aa');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Abkhazian', 'ab');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Afrikaans', 'af');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Amharic', 'am');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Arabic', 'ar');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Assamese', 'as');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Aymara', 'ay');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Azerbaijani', 'az');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Bashkir', 'ba');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Byelorussian', 'be');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Bulgarian', 'bg');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Bihari', 'bh');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Bislama', 'bi');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Bengali/Bangla', 'bn');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Tibetan', 'bo');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Breton', 'br');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Catalan', 'ca');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Corsican', 'co');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Czech', 'cs');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Welsh', 'cy');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Danish', 'da');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('German', 'de');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Bhutani', 'dz');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Greek', 'el');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Esperanto', 'eo');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Spanish', 'es');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Estonian', 'et');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Basque', 'eu');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Persian', 'fa');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Finnish', 'fi');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Fiji', 'fj');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Faeroese', 'fo');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('French', 'fr');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Frisian', 'fy');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Irish', 'ga');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Scots/Gaelic', 'gd');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Galician', 'gl');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Guarani', 'gn');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Gujarati', 'gu');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Hausa', 'ha');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Hindi', 'hi');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Croatian', 'hr');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Hungarian', 'hu');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Armenian', 'hy');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Interlingua', 'ia');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Interlingue', 'ie');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Inupiak', 'ik');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Indonesian', 'in');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Icelandic', 'is');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Italian', 'it');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Hebrew', 'iw');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Japanese', 'ja');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Yiddish', 'ji');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Javanese', 'jw');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Georgian', 'ka');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Kazakh', 'kk');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Greenlandic', 'kl');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Cambodian', 'km');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Kannada', 'kn');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Korean', 'ko');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Kashmiri', 'ks');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Kurdish', 'ku');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Kirghiz', 'ky');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Latin', 'la');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Lingala', 'ln');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Laothian', 'lo');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Lithuanian', 'lt');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Latvian/Lettish', 'lv');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Malagasy', 'mg');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Maori', 'mi');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Macedonian', 'mk');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Malayalam', 'ml');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Mongolian', 'mn');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Moldavian', 'mo');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Marathi', 'mr');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Malay', 'ms');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Maltese', 'mt');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Burmese', 'my');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Nauru', 'na');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Nepali', 'ne');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Dutch', 'nl');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Norwegian', 'no');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Occitan', 'oc');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('(Afan)/Oromoor/Oriya', 'om');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Punjabi', 'pa');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Polish', 'pl');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Pashto/Pushto', 'ps');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Portuguese', 'pt');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Quechua', 'qu');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Rhaeto-Romance', 'rm');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Kirundi', 'rn');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Romanian', 'ro');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Russian', 'ru');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Kinyarwanda', 'rw');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Sanskrit', 'sa');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Sindhi', 'sd');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Sangro', 'sg');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Serbo-Croatian', 'sh');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Singhalese', 'si');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Slovak', 'sk');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Slovenian', 'sl');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Samoan', 'sm');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Shona', 'sn');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Somali', 'so');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Albanian', 'sq');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Serbian', 'sr');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Siswati', 'ss');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Sesotho', 'st');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Sundanese', 'su');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Swedish', 'sv');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Swahili', 'sw');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Tamil', 'ta');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Tegulu', 'te');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Tajik', 'tg');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Thai', 'th');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Tigrinya', 'ti');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Turkmen', 'tk');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Tagalog', 'tl');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Setswana', 'tn');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Tonga', 'to');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Turkish', 'tr');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Tsonga', 'ts');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Tatar', 'tt');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Twi', 'tw');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Ukrainian', 'uk');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Urdu', 'ur');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Uzbek', 'uz');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Vietnamese', 'vi');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Volapuk', 'vo');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Wolof', 'wo');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Xhosa', 'xh');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Yoruba', 'yo');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Chinese', 'zh');
INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('Zulu', 'zu');

INSERT INTO wtvmT_StreamStatus (Code, Label, Color, Searchable) VALUES
	(1, 'Submitted', 'black', TRUE),
	(2, 'Working', 'green', TRUE),
	(3, 'Deprecated', 'orangered', TRUE),
	(4, 'Duplicate', 'orange', TRUE),
	(5, 'Invalid', 'gray', TRUE),
	(6, 'Dead', 'darkred', TRUE),
	(7, 'Forbidden', 'red', FALSE);

DROP TABLE IF EXISTS wtvmT_AuthAssignment;
DROP TABLE IF EXISTS wtvmT_AuthItemChild;
DROP TABLE IF EXISTS wtvmT_AuthItem;

CREATE TABLE wtvmT_AuthItem
(
   name                 VARCHAR(64) NOT NULL,
   type                 INTEGER NOT NULL,
   description          text,
   bizrule              text,
   data                 text,
   primary key (name)
);

CREATE TABLE wtvmT_AuthItemChild
(
   parent               VARCHAR(64) NOT NULL,
   child                VARCHAR(64) NOT NULL,
   primary key (parent,child),
   foreign key (parent) references AuthItem (name) ON DELETE CASCADE ON UPDATE CASCADE,
   foreign key (child) references AuthItem (name) ON DELETE CASCADE ON UPDATE CASCADE
);

CREATE TABLE wtvmT_AuthAssignment
(
   itemname             VARCHAR(64) NOT NULL,
   userid               VARCHAR(64) NOT NULL,
   bizrule              text,
   data                 text,
   primary key (itemname,userid),
   foreign key (itemname) references AuthItem (name) ON DELETE CASCADE ON UPDATE CASCADE
);

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
		REFERENCES wtvmT_User(Id)
) TYPE=InnoDB;

ALTER TABLE wtvmT_WebStream MODIFY COLUMN Name VARCHAR(100);

SET foreign_key_checks = 0;
ALTER TABLE wtvmT_Country CONVERT TO CHARACTER SET utf8;
ALTER TABLE wtvmT_History CONVERT TO CHARACTER SET utf8;
ALTER TABLE wtvmT_Lang CONVERT TO CHARACTER SET utf8;
ALTER TABLE wtvmT_StreamStatus CONVERT TO CHARACTER SET utf8;
ALTER TABLE wtvmT_TVChannel CONVERT TO CHARACTER SET utf8;
ALTER TABLE wtvmT_TVChannelType CONVERT TO CHARACTER SET utf8;
ALTER TABLE wtvmT_User CONVERT TO CHARACTER SET utf8;
ALTER TABLE wtvmT_WebStream CONVERT TO CHARACTER SET utf8;
SET foreign_key_checks = 1;

-- Version 0.2.0

ALTER TABLE wtvmT_WebStream ADD CountryCode CHAR(2) NULL AFTER LangCode;
ALTER TABLE wtvmT_WebStream ADD CONSTRAINT FK_WebStream_Country FOREIGN KEY (CountryCode) REFERENCES wtvmT_Country (Code);

INSERT INTO `wtvmT_Country` (`Code`, `Label`) VALUES
	('ad', 'Andorra'),
	('ae', 'United Arab Emirates'),
	('af', 'Afghanistan'),
	('ag', 'Antigua and Barbuda'),
	('ai', 'Anguilla'),
	('al', 'Albania'),
	('am', 'Armenia'),
	('an', 'Netherlands Antilles'),
	('ao', 'Angola'),
	('aq', 'Antarctica'),
	('ar', 'Argentina'),
	('as', 'American Samoa'),
	('at', 'Austria'),
	('au', 'Australia'),
	('aw', 'Aruba'),
	('az', 'Azerbaijan'),
	('ba', 'Bosnia and Herzegovi'),
	('bb', 'Barbados'),
	('bd', 'Bangladesh'),
	('be', 'Belgium'),
	('bf', 'Burkina Faso'),
	('bg', 'Bulgaria'),
	('bh', 'Bahrain'),
	('bi', 'Burundi'),
	('bj', 'Benin'),
	('bm', 'Bermuda'),
	('bn', 'Brunei Darussalam'),
	('bo', 'Bolivia'),
	('br', 'Brazil'),
	('bs', 'Bahamas'),
	('bt', 'Bhutan'),
	('bv', 'Bouvet Island'),
	('bw', 'Botswana'),
	('by', 'Belarus'),
	('bz', 'Belize'),
	('ca', 'Canada'),
	('cc', 'Cocos (Keeling) Isla'),
	('cd', 'Congo, the Democrati'),
	('cf', 'Central African Repu'),
	('cg', 'Congo'),
	('ch', 'Switzerland'),
	('ci', 'Cote D''Ivoire'),
	('ck', 'Cook Islands'),
	('cl', 'Chile'),
	('cm', 'Cameroon'),
	('cn', 'China'),
	('co', 'Colombia'),
	('cr', 'Costa Rica'),
	('cs', 'Serbia and Montenegr'),
	('cu', 'Cuba'),
	('cv', 'Cape Verde'),
	('cx', 'Christmas Island'),
	('cy', 'Cyprus'),
	('cz', 'Czech Republic'),
	('de', 'Germany'),
	('dj', 'Djibouti'),
	('dk', 'Denmark'),
	('dm', 'Dominica'),
	('do', 'Dominican Republic'),
	('dz', 'Algeria'),
	('ec', 'Ecuador'),
	('ee', 'Estonia'),
	('eg', 'Egypt'),
	('eh', 'Western Sahara'),
	('er', 'Eritrea'),
	('es', 'Spain'),
	('et', 'Ethiopia'),
	('fi', 'Finland'),
	('fj', 'Fiji'),
	('fk', 'Falkland Islands (Ma'),
	('fm', 'Micronesia, Federate'),
	('fo', 'Faroe Islands'),
	('fr', 'France'),
	('ga', 'Gabon'),
	('gb', 'United Kingdom'),
	('gd', 'Grenada'),
	('ge', 'Georgia'),
	('gf', 'French Guiana'),
	('gh', 'Ghana'),
	('gi', 'Gibraltar'),
	('gl', 'Greenland'),
	('gm', 'Gambia'),
	('gn', 'Guinea'),
	('gp', 'Guadeloupe'),
	('gq', 'Equatorial Guinea'),
	('gr', 'Greece'),
	('gs', 'South Georgia and th'),
	('gt', 'Guatemala'),
	('gu', 'Guam'),
	('gw', 'Guinea-Bissau'),
	('gy', 'Guyana'),
	('hk', 'Hong Kong'),
	('hm', 'Heard Island and Mcd'),
	('hn', 'Honduras'),
	('hr', 'Croatia'),
	('ht', 'Haiti'),
	('hu', 'Hungary'),
	('id', 'Indonesia'),
	('ie', 'Ireland'),
	('il', 'Israel'),
	('in', 'India'),
	('io', 'British Indian Ocean'),
	('iq', 'Iraq'),
	('ir', 'Iran, Islamic Republ'),
	('is', 'Iceland'),
	('it', 'Italy'),
	('jm', 'Jamaica'),
	('jo', 'Jordan'),
	('jp', 'Japan'),
	('ke', 'Kenya'),
	('kg', 'Kyrgyzstan'),
	('kh', 'Cambodia'),
	('ki', 'Kiribati'),
	('km', 'Comoros'),
	('kn', 'Saint Kitts and Nevi'),
	('kp', 'Korea, Democratic Pe'),
	('kr', 'Korea, Republic of'),
	('kw', 'Kuwait'),
	('ky', 'Cayman Islands'),
	('kz', 'Kazakhstan'),
	('la', 'Lao People''s Democra'),
	('lb', 'Lebanon'),
	('lc', 'Saint Lucia'),
	('li', 'Liechtenstein'),
	('lk', 'Sri Lanka'),
	('lr', 'Liberia'),
	('ls', 'Lesotho'),
	('lt', 'Lithuania'),
	('lu', 'Luxembourg'),
	('lv', 'Latvia'),
	('ly', 'Libyan Arab Jamahiri'),
	('ma', 'Morocco'),
	('mc', 'Monaco'),
	('md', 'Moldova, Republic of'),
	('mg', 'Madagascar'),
	('mh', 'Marshall Islands'),
	('mk', 'Macedonia, the Forme'),
	('ml', 'Mali'),
	('mm', 'Myanmar'),
	('mn', 'Mongolia'),
	('mo', 'Macao'),
	('mp', 'Northern Mariana Isl'),
	('mq', 'Martinique'),
	('mr', 'Mauritania'),
	('ms', 'Montserrat'),
	('mt', 'Malta'),
	('mu', 'Mauritius'),
	('mv', 'Maldives'),
	('mw', 'Malawi'),
	('mx', 'Mexico'),
	('my', 'Malaysia'),
	('mz', 'Mozambique'),
	('na', 'Namibia'),
	('nc', 'New Caledonia'),
	('ne', 'Niger'),
	('nf', 'Norfolk Island'),
	('ng', 'Nigeria'),
	('ni', 'Nicaragua'),
	('nl', 'Netherlands'),
	('no', 'Norway'),
	('np', 'Nepal'),
	('nr', 'Nauru'),
	('nu', 'Niue'),
	('nz', 'New Zealand'),
	('om', 'Oman'),
	('pa', 'Panama'),
	('pe', 'Peru'),
	('pf', 'French Polynesia'),
	('pg', 'Papua New Guinea'),
	('ph', 'Philippines'),
	('pk', 'Pakistan'),
	('pl', 'Poland'),
	('pm', 'Saint Pierre and Miq'),
	('pn', 'Pitcairn'),
	('pr', 'Puerto Rico'),
	('ps', 'Palestinian Territor'),
	('pt', 'Portugal'),
	('pw', 'Palau'),
	('py', 'Paraguay'),
	('qa', 'Qatar'),
	('re', 'Reunion'),
	('ro', 'Romania'),
	('ru', 'Russian Federation'),
	('rw', 'Rwanda'),
	('sa', 'Saudi Arabia'),
	('sb', 'Solomon Islands'),
	('sc', 'Seychelles'),
	('sd', 'Sudan'),
	('se', 'Sweden'),
	('sg', 'Singapore'),
	('sh', 'Saint Helena'),
	('si', 'Slovenia'),
	('sj', 'Svalbard and Jan May'),
	('sk', 'Slovakia'),
	('sl', 'Sierra Leone'),
	('sm', 'San Marino'),
	('sn', 'Senegal'),
	('so', 'Somalia'),
	('sr', 'Suriname'),
	('st', 'Sao Tome and Princip'),
	('sv', 'El Salvador'),
	('sy', 'Syrian Arab Republic'),
	('sz', 'Swaziland'),
	('tc', 'Turks and Caicos Isl'),
	('td', 'Chad'),
	('tf', 'French Southern Terr'),
	('tg', 'Togo'),
	('th', 'Thailand'),
	('tj', 'Tajikistan'),
	('tk', 'Tokelau'),
	('tl', 'Timor-Leste'),
	('tm', 'Turkmenistan'),
	('tn', 'Tunisia'),
	('to', 'Tonga'),
	('tr', 'Turkey'),
	('tt', 'Trinidad and Tobago'),
	('tv', 'Tuvalu'),
	('tw', 'Taiwan, Province of '),
	('tz', 'Tanzania, United Rep'),
	('ua', 'Ukraine'),
	('ug', 'Uganda'),
	('um', 'United States Minor '),
	('us', 'United States'),
	('uy', 'Uruguay'),
	('uz', 'Uzbekistan'),
	('va', 'Holy See (Vatican Ci'),
	('vc', 'Saint Vincent and th'),
	('ve', 'Venezuela'),
	('vg', 'Virgin Islands, Brit'),
	('vi', 'Virgin Islands, U.s.'),
	('vn', 'Viet Nam'),
	('vu', 'Vanuatu'),
	('wf', 'Wallis and Futuna'),
	('ws', 'Samoa'),
	('ye', 'Yemen'),
	('yt', 'Mayotte'),
	('za', 'South Africa'),
	('zm', 'Zambia'),
	('zw', 'Zimbabwe');

