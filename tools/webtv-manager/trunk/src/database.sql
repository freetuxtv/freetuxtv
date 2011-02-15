-- ALTER DATABASE freetuxtv CHARACTER SET utf8;

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

INSERT INTO wtvmT_Lang (Label, Code)  VALUES ('English', 'en'),('Afar', 'aa'),('Abkhazian', 'ab'),('Afrikaans', 'af'),('Amharic', 'am'),('Arabic', 'ar'),('Assamese', 'as'),('Aymara', 'ay'),('Azerbaijani', 'az'),('Bashkir', 'ba'),('Byelorussian', 'be'),('Bulgarian', 'bg'),('Bihari', 'bh'),('Bislama', 'bi'),('Bengali/Bangla', 'bn'),('Tibetan', 'bo'),('Breton', 'br'),('Catalan', 'ca'),('Corsican', 'co'),('Czech', 'cs'),('Welsh', 'cy'),('Danish', 'da'),('German', 'de'),('Bhutani', 'dz'),('Greek', 'el'),('Esperanto', 'eo'),('Spanish', 'es'),('Estonian', 'et'),('Basque', 'eu'),('Persian', 'fa'),('Finnish', 'fi'),('Fiji', 'fj'),('Faeroese', 'fo'),('French', 'fr'),('Frisian', 'fy'),('Irish', 'ga'),('Scots/Gaelic', 'gd'),('Galician', 'gl'),('Guarani', 'gn'),('Gujarati', 'gu'),('Hausa', 'ha'),('Hindi', 'hi'),('Croatian', 'hr'),('Hungarian', 'hu'),('Armenian', 'hy'),('Interlingua', 'ia'),('Interlingue', 'ie'),('Inupiak', 'ik'),('Indonesian', 'in'),('Icelandic', 'is'),('Italian', 'it'),('Hebrew', 'iw'),('Japanese', 'ja'),('Yiddish', 'ji'),('Javanese', 'jw'),('Georgian', 'ka'),('Kazakh', 'kk'),('Greenlandic', 'kl'),('Cambodian', 'km'),('Kannada', 'kn'),('Korean', 'ko'),('Kashmiri', 'ks'),('Kurdish', 'ku'),('Kirghiz', 'ky'),('Latin', 'la'),('Lingala', 'ln'),('Laothian', 'lo'),('Lithuanian', 'lt'),('Latvian/Lettish', 'lv'),('Malagasy', 'mg'),('Maori', 'mi'),('Macedonian', 'mk'),('Malayalam', 'ml'),('Mongolian', 'mn'),('Moldavian', 'mo'),('Marathi', 'mr'),('Malay', 'ms'),('Maltese', 'mt'),('Burmese', 'my'),('Nauru', 'na'),('Nepali', 'ne'),('Dutch', 'nl'),('Norwegian', 'no'),('Occitan', 'oc'),('(Afan)/Oromoor/Oriya', 'om'),('Punjabi', 'pa'),('Polish', 'pl'),('Pashto/Pushto', 'ps'),('Portuguese', 'pt'),('Quechua', 'qu'),('Rhaeto-Romance', 'rm'),('Kirundi', 'rn'),('Romanian', 'ro'),('Russian', 'ru'),('Kinyarwanda', 'rw'),('Sanskrit', 'sa'),('Sindhi', 'sd'),('Sangro', 'sg'),('Serbo-Croatian', 'sh'),('Singhalese', 'si'),('Slovak', 'sk'),('Slovenian', 'sl'),('Samoan', 'sm'),('Shona', 'sn'),('Somali', 'so'),('Albanian', 'sq'),('Serbian', 'sr'),('Siswati', 'ss'),('Sesotho', 'st'),('Sundanese', 'su'),('Swedish', 'sv'),('Swahili', 'sw'),('Tamil', 'ta'),('Tegulu', 'te'),('Tajik', 'tg'),('Thai', 'th'),('Tigrinya', 'ti'),('Turkmen', 'tk'),('Tagalog', 'tl'),('Setswana', 'tn'),('Tonga', 'to'),('Turkish', 'tr'),('Tsonga', 'ts'),('Tatar', 'tt'),('Twi', 'tw'),('Ukrainian', 'uk'),('Urdu', 'ur'),('Uzbek', 'uz'),('Vietnamese', 'vi'),('Volapuk', 'vo'),('Wolof', 'wo'),('Xhosa', 'xh'),('Yoruba', 'yo'),('Chinese', 'zh'),('Zulu', 'zu');

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
		REFERENCES wtvmT_YumUsers(id)
) TYPE=InnoDB;


ALTER TABLE wtvmT_WebStream MODIFY COLUMN Name VARCHAR(100);

SET foreign_key_checks = 0;
ALTER TABLE wtvmT_Country CONVERT TO CHARACTER SET utf8;
ALTER TABLE wtvmT_History CONVERT TO CHARACTER SET utf8;
ALTER TABLE wtvmT_Lang CONVERT TO CHARACTER SET utf8;
ALTER TABLE wtvmT_StreamStatus CONVERT TO CHARACTER SET utf8;
ALTER TABLE wtvmT_TVChannel CONVERT TO CHARACTER SET utf8;
ALTER TABLE wtvmT_TVChannelType CONVERT TO CHARACTER SET utf8;
ALTER TABLE wtvmT_WebStream CONVERT TO CHARACTER SET utf8;
SET foreign_key_checks = 1;

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

/* IF EXISTS wtvmT_User (
	insert into  wtvmT_YumUsers (username,password,superuser,status) 
	SELECT Username, Password, superuser=0, status = 1
	FROM wtvmT_User,wtvmT_YumUsers
	
	insert into  wtvmT_YumProfiles (user_id,timestamp,privacy, lastname,firstname,email) 
	 wtvmT_YumUsers (username,password,superuser,status) 
	SELECT YU.id, timestamp=0, privacy=protected, YU.username,firstname= "", Us.Email
	FROM wtvmT_YumUsers as YU, wtvmT_User as Us, wtvmT_YumProfiles
	WHERE YU.username=Us.Username and YU.password=Us.Password
);
 */