-- Table for the TV Channel
CREATE TABLE IF NOT EXISTS tvchannel (
   id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
   name VARCHAR(50) NOT NULL,
   logo_filename VARCHAR(20)
);

-- Table for alternative name for TV Channel 
CREATE TABLE IF NOT EXISTS label_tvchannel (
   id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
   label VARCHAR(50) NOT NULL,
   tvchannel_id INTEGER NOT NULL
     CONSTRAINT fk_labeltvchannel_tvchannelid REFERENCES tvchannel(id) ON DELETE CASCADE
);

-- Table for the group of Channel
CREATE TABLE IF NOT EXISTS channels_group (
   id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
   rank INTEGER NOT NULL,
   name VARCHAR(50) NOT NULL,
   type INTEGER NOT NULL,
   uri VARCHAR(500) NULL,
   bregex VARCHAR(50) NULL,
   eregex VARCHAR(50) NULL
);

-- Table for the Channel
CREATE TABLE IF NOT EXISTS channel (
   id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
   name VARCHAR(50) NOT NULL,
   rank INTEGER NOT NULL,
   uri VARCHAR(255) NOT NULL,
   channelsgroup_id INTEGER NOT NULL
     CONSTRAINT fk_channel_channelsgroupid REFERENCES channels_group(id) ON DELETE CASCADE,
   tvchannel_id INTEGER NULL
     CONSTRAINT fk_channel_tvchannelid REFERENCES tvchannel(id) ON DELETE SET NULL
);

CREATE TRIGGER fkd_delele_tvchannel
  BEFORE DELETE ON tvchannel
  FOR EACH ROW BEGIN
      UPDATE channel SET tvchannel_id = NULL WHERE tvchannel_id = OLD.id;
      DELETE FROM label_tvchannel WHERE tvchannel_id = OLD.id;
  END;

CREATE TRIGGER fkd_delete_channelsgroup
  BEFORE DELETE ON channels_group
  FOR EACH ROW BEGIN
      DELETE from channel WHERE channelsgroup_id = OLD.id;
  END;

