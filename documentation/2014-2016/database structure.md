

# Database structure

When player registers at forum, new row will be added to the "th3forum" database, where GUID will be saved and other things you need.
That GUID is required by me in "thelastofus" database in ` characters_data `, where characters_data will hold all the characters created by players. As soon as player creates new character, that (new) character will receive default settings (x, y, z etc.) sent by forum MySQL.

# Side note as of 2020-02-08
This logic is probably out-of-date. Something similar might have been implemented since 2015 so it is recommended to look-up actual database structure in the SQL file:
https://github.com/Jedrzej94/tlou-sa-mp-/blob/0_3_7_R2-2_sampctl/037_R2-2_WIN/sql/tlou_prd.sql

# player_data structure
| [Field name]  | [Description]                                                      |
|---------------|--------------------------------------------------------------------|
| GUID          | Data sent from forum (register account global UID).                |
| password      | Data sent from forum with GUID (as above).                         |
| admin_level   | by default: 0.                                                     |
| volume_level  | by default: 100.                                                   |
| status        | [0 (default) = offline, 1 = online, 2 = admin jailed, 3 = banned]. |
| register_date | Data sent from forum (date when player registered).                |
|               |                                                                    |

# characters_data structure
| [Field name] | [Description]                                                      |
|--------------|--------------------------------------------------------------------|
| GUID         | sent from forum, when user registers.                              |
| UID          | Auto_increment - sent from forum, when player creates a character. |
| name         | name of the character, set in forum.                               |
| health       | 100.0 by default.                                                  |
| skinid       | chosen at forum.                                                   |
| LastX        | random from table that I will create.                              |
| LastY        | random from table that I will create.                              |
| LastZ        | random from table that I will create.                              |
| LastA        | random from table that I will create.                              |
| LastINT      | random from table that I will create.                              |
| LastVW       | random from table that I will create.                              |
|              |                                                                    |

# items_data structure
| [Field name]  | [Description]                                                    |
|---------------|------------------------------------------------------------------|
| UID           | Auto_increment - UID of the object (managed by gamemode script). |
| objectid      | ...                                                              |
| name          | All managed by script.                                           |
| type          | All managed by script.                                           |
| model         | All managed by script.                                           |
| x             | All managed by script.                                           |
| y             | All managed by script.                                           |
| z             | All managed by script.                                           |
| rX            | All managed by script.                                           |
| rY            | All managed by script.                                           |
| rZ            | All managed by script.                                           |
| virtual_world | All managed by script.                                           |
| interior      | All managed by script.                                           |
| quantity      | All managed by script.                                           |
| owner_uid     | All managed by script.                                           |
|||