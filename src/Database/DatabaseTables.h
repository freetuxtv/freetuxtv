//
// Created by ebeuque on 08/02/25.
//

#ifndef FREETUXTV_DATABASE_DATABASETABLES_H
#define FREETUXTV_DATABASE_DATABASETABLES_H

#define SQLITE_FORMAT_DATETIME          "%Y-%m-%d %H:%M:%S"
#define SQLITE_FORMAT_DATETIME00        "%Y-%m-%d %H:%M:00"

// Define constant for the list of field in the database

// Table tvchannel
#define DB_TVCHANNEL                    "tvchannel"
#define DB_TVCHANNEL_ID                 "id"
#define DB_TVCHANNEL_NAME               "name"
#define DB_TVCHANNEL_LOGOFILENAME       "logo_filename"

// Table label_tvchannel
#define DB_LABELTVCHANNEL               "label_tvchannel"
#define DB_LABELTVCHANNEL_ID            "id"
#define DB_LABELTVCHANNEL_LABEL         "label"
#define DB_LABELTVCHANNEL_TVCHANNELID   "tvchannel_id"

// Table channels_group
#define DB_CHANNELSGROUP                "channels_group"
#define DB_CHANNELSGROUP_ID             "id"
#define DB_CHANNELSGROUP_POSITION       "position"
#define DB_CHANNELSGROUP_NAME           "name"
#define DB_CHANNELSGROUP_TYPE           "type"
#define DB_CHANNELSGROUP_URI            "uri"
#define DB_CHANNELSGROUP_BREGEX         "bregex"
#define DB_CHANNELSGROUP_EREGEX         "eregex"
#define DB_CHANNELSGROUP_LASTUPDATE     "last_update"

// Table channel
#define DB_CHANNEL                     "channel"
#define DB_CHANNEL_ID                  "id"
#define DB_CHANNEL_NAME                "name"
#define DB_CHANNEL_POSITION            "position"
#define DB_CHANNEL_URI                 "uri"
#define DB_CHANNEL_VLCOPTIONS          "vlc_options"
#define DB_CHANNEL_DEINTERLACEMODE     "deinterlace_mode"
#define DB_CHANNEL_UPDATED             "updated"
#define DB_CHANNEL_CHANNELGROUPID      "channelsgroup_id"
#define DB_CHANNEL_TVCHANNELID         "tvchannel_id"

// Table recording
#define DB_RECORDING                    "recording"
#define DB_RECORDING_ID                 "id"
#define DB_RECORDING_TITLE              "title"
#define DB_RECORDING_STATUS             "status"
#define DB_RECORDING_BEGINTIME	        "begin_date"
#define DB_RECORDING_ENDTIME	        "end_date"
#define DB_RECORDING_FILENAME	        "filename"
#define DB_RECORDING_CHANNELID	        "channel_id"

#endif //FREETUXTV_DATABASE_DATABASETABLES_H
