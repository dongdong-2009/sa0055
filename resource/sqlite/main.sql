PRAGMA foreign_keys=OFF;
BEGIN TRANSACTION;
CREATE TABLE "sa0055_data_source_enum" (
"id"  INTEGER NOT NULL,
"name"  TEXT(256) NOT NULL,
PRIMARY KEY ("id" ASC)
);
INSERT INTO "sa0055_data_source_enum" VALUES(0,'装置');
INSERT INTO "sa0055_data_source_enum" VALUES(1,'计算量');
INSERT INTO "sa0055_data_source_enum" VALUES(2,'积分量');
CREATE TABLE "sa0055_para_type_enum" (
"id"  INTEGER NOT NULL,
"name"  TEXT(256),
PRIMARY KEY ("id" ASC)
);
INSERT INTO "sa0055_para_type_enum" VALUES(0,'字符串');
INSERT INTO "sa0055_para_type_enum" VALUES(1,'整数');
INSERT INTO "sa0055_para_type_enum" VALUES(2,'浮点数');
INSERT INTO "sa0055_para_type_enum" VALUES(3,'布尔值');
CREATE TABLE "sa0055_device_run_mode_enum" (
"id"  INTEGER NOT NULL,
"name"  TEXT(256),
PRIMARY KEY ("id")
);
INSERT INTO "sa0055_device_run_mode_enum" VALUES(0,'单机');
INSERT INTO "sa0055_device_run_mode_enum" VALUES(1,'主备冗余');
CREATE TABLE "sa0055_ai_frame_type_enum" (
"id"  INTEGER,
"name"  TEXT(256),
PRIMARY KEY ("id")
);
INSERT INTO "sa0055_ai_frame_type_enum" VALUES(5,'步位置信息(5)');
INSERT INTO "sa0055_ai_frame_type_enum" VALUES(9,'规一化测量值(9)');
INSERT INTO "sa0055_ai_frame_type_enum" VALUES(11,'标度化测量值(11)');
INSERT INTO "sa0055_ai_frame_type_enum" VALUES(13,'短浮点测量值(13)');
INSERT INTO "sa0055_ai_frame_type_enum" VALUES(21,'无品质位规一化测量值(21)');
CREATE TABLE "sa0055_ao_frame_type_enum" (
"id"  INTEGER,
"name"  TEXT(256),
PRIMARY KEY ("id")
);
INSERT INTO "sa0055_ao_frame_type_enum" VALUES(48,'规一化设定值命令(48)');
INSERT INTO "sa0055_ao_frame_type_enum" VALUES(49,'标度化设定值命令(49)');
INSERT INTO "sa0055_ao_frame_type_enum" VALUES(50,'短浮点设定值命令(50)');
CREATE TABLE "sa0055_ci_frame_type_enum" (
"id"  INTEGER,
"name"  TEXT(256),
PRIMARY KEY ("id")
);
INSERT INTO "sa0055_ci_frame_type_enum" VALUES(15,'累计量(15)');
CREATE TABLE "sa0055_di_frame_type_enum" (
"id"  INTEGER,
"name"  TEXT(256),
PRIMARY KEY ("id")
);
INSERT INTO "sa0055_di_frame_type_enum" VALUES(1,'单点遥信(1)');
INSERT INTO "sa0055_di_frame_type_enum" VALUES(3,'双点遥信(3)');
INSERT INTO "sa0055_di_frame_type_enum" VALUES(7,'32比特串(7)');
INSERT INTO "sa0055_di_frame_type_enum" VALUES(20,'带变位检出成组单点(20)');
CREATE TABLE "sa0055_do_frame_type_enum" (
"id"  INTEGER,
"name"  TEXT(256),
PRIMARY KEY ("id")
);
INSERT INTO "sa0055_do_frame_type_enum" VALUES(45,'单点命令(45)');
INSERT INTO "sa0055_do_frame_type_enum" VALUES(46,'双点命令(46)');
INSERT INTO "sa0055_do_frame_type_enum" VALUES(47,'步调节命令(47)');
CREATE TABLE "sa0055_iec104_byte_num_enum" (
"id"  INTEGER NOT NULL,
"name"  TEXT,
PRIMARY KEY ("id")
);
INSERT INTO "sa0055_iec104_byte_num_enum" VALUES(1,'1字节');
INSERT INTO "sa0055_iec104_byte_num_enum" VALUES(2,'2字节');
INSERT INTO "sa0055_iec104_byte_num_enum" VALUES(3,'3字节');
ANALYZE sqlite_master;
CREATE TABLE "config_ao_point" (
"seq_num"  INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
"device_id"  INTEGER NOT NULL,
"ao_id"  INTEGER NOT NULL,
"ao_point"  INTEGER DEFAULT -1
);
CREATE TABLE "config_ci_point" (
"seq_num"  INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
"device_id"  INTEGER NOT NULL,
"ci_id"  INTEGER NOT NULL,
"ci_point"  INTEGER DEFAULT -1
);
CREATE TABLE `sa0055_curve_calc_method_enum` (
	`id`	INTEGER,
	`name`	TEXT,
	PRIMARY KEY(`id`)
);
INSERT INTO "sa0055_curve_calc_method_enum" VALUES(0,'直线');
INSERT INTO "sa0055_curve_calc_method_enum" VALUES(1,'斜线');
CREATE TABLE `sa0055_curve_unit_enum` (
	`id`	INTEGER,
	`name`	TEXT,
	PRIMARY KEY(`id`)
);
INSERT INTO "sa0055_curve_unit_enum" VALUES(0,'kV');
INSERT INTO "sa0055_curve_unit_enum" VALUES(1,'MVar');
INSERT INTO "sa0055_curve_unit_enum" VALUES(2,'MW');
CREATE TABLE "sa0055_curve_type_enum" (
	`id`	INTEGER,
	`name`	TEXT,
	PRIMARY KEY(`id`)
);
INSERT INTO "sa0055_curve_type_enum" VALUES(0,'96点');
INSERT INTO "sa0055_curve_type_enum" VALUES(1,'288点');
CREATE TABLE "config_formula_definition" (
"seq_num"  INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
"formula_id"  INTEGER DEFAULT -1,
"formula_type"  INTEGER DEFAULT 1,
"formula_str"  TEXT DEFAULT '',
"variable_num"  INTEGER DEFAULT 0
);
CREATE TABLE "config_alarm_rule" (
"seq_num"  INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
"alarm_rule_id"  INTEGER DEFAULT -1,
"alarm_cause"  INTEGER DEFAULT 0,
"alarm_sub_cause"  INTEGER DEFAULT 0,
"alarm_type"  INTEGER DEFAULT 0,
"cos_display"  INTEGER DEFAULT 0
);
CREATE TABLE "config_alarm_info" (
"seq_num"  INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
"alarm_rule_id"  INTEGER NOT NULL DEFAULT -1,
"device_id"  INTEGER DEFAULT -1,
"point_type"  INTEGER DEFAULT -1,
"point_desc_id"  INTEGER DEFAULT -1,
"ai_upper_limit"  REAL DEFAULT 32767,
"ai_low_limit"  REAL DEFAULT -32768,
"ai_upper_upper_limit"  REAL DEFAULT 99999999,
"ai_low_low_limit"  REAL DEFAULT -99999999,
"must_confirm"  INTEGER DEFAULT 0,
"unconfirmed_color"  INTEGER DEFAULT 16711680,
"confirmed_color"  INTEGER DEFAULT 16733440,
"sound_file_path"  TEXT,
"sound_play_time"  INTEGER DEFAULT 0,
"print_alarm"  INTEGER DEFAULT 0,
"push_picture_path"  TEXT
);
CREATE TABLE "config_sound_file" (
"sound_file_path"  TEXT NOT NULL
);
CREATE TABLE "config_picture_file" (
"picture_file_path"  TEXT
);
CREATE TABLE "config_ai_point" (
"seq_num"  INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
"device_id"  INTEGER NOT NULL,
"ai_id"  INTEGER NOT NULL,
"ai_point"  INTEGER DEFAULT -1,
"multiple"  REAL DEFAULT 1,
"offset"  REAL DEFAULT 0,
"dump_flag"  INTEGER DEFAULT 0,
"dump_interval"  INTEGER DEFAULT 5,
"trace_flag"  INTEGER DEFAULT 0,
"deadband"  REAL DEFAULT 0.0,
"source"  INTEGER DEFAULT 0
);
CREATE TABLE "config_alarm_rule_map" (
"id"  INTEGER NOT NULL,
"name"  TEXT(256) NOT NULL,
"valid"  INTEGER NOT NULL DEFAULT 1,
PRIMARY KEY ("id" ASC)
);
CREATE TABLE "config_calc_var_bind" (
"calc_id"  INTEGER,
"var_seq_num"  INTEGER,
"device_id"  INTEGER,
"point_type"  INTEGER,
"point_id"  INTEGER
);
CREATE TABLE "config_calc_var_map" (
"id"  INTEGER NOT NULL,
"name"  TEXT NOT NULL,
"valid"  INTEGER NOT NULL
);
CREATE TABLE "config_di_point" (
"seq_num"  INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
"device_id"  INTEGER NOT NULL,
"di_id"  INTEGER NOT NULL,
"di_point"  INTEGER DEFAULT -1,
"source"  INTEGER DEFAULT 0
);
CREATE TABLE "config_do_point" (
"seq_num"  INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
"device_id"  INTEGER NOT NULL,
"do_id"  INTEGER NOT NULL,
"do_point"  INTEGER DEFAULT -1,
"qualifier"  INTEGER DEFAULT 0,
"do_type"  INTEGER DEFAULT 1,
"step_type"  INTEGER DEFAULT 0
);
CREATE TABLE "config_global_para" (
"para_id"  INTEGER,
"para_name"  TEXT(256) NOT NULL,
"para_value"  TEXT(256) NOT NULL,
"para_type"  INTEGER NOT NULL,
PRIMARY KEY ("para_name" ASC)
);
CREATE TABLE "config_iec104_para" (
"seq_num"  INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
"device_id"  INTEGER NOT NULL,
"asdu_addr"  INTEGER DEFAULT 1,
"cot_size"  INTEGER DEFAULT 2,
"adsu_size"  INTEGER DEFAULT 2,
"ioa_size"  INTEGER DEFAULT 3,
"di_ioa_start"  INTEGER DEFAULT 1,
"ai_ioa_start"  INTEGER DEFAULT 16385,
"do_ioa_start"  INTEGER DEFAULT 24577,
"ao_ioa_start"  INTEGER DEFAULT 25089,
"ci_ioa_start"  INTEGER DEFAULT 25601,
"di_type_id"  INTEGER DEFAULT 1,
"ai_type_id"  INTEGER DEFAULT 9,
"do_type_id"  INTEGER DEFAULT 45,
"ao_type_id"  INTEGER DEFAULT 48,
"ci_type_id"  INTEGER DEFAULT 15
);
CREATE TABLE "config_integral_definition" (
"seq_num"  INTEGER PRIMARY KEY AUTOINCREMENT,
"integral_id"  INTEGER,
"sampling_interval"  INTEGER DEFAULT 1,
"write_db_interval"  INTEGER DEFAULT 300,
"clear_flag"  INTEGER DEFAULT 0,
"device_id"  INTEGER DEFAULT -1,
"ai_id"  INTEGER DEFAULT -1
);
CREATE TABLE "config_plan_curve" (
"seq_num"  INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
"curve_id"  INTEGER DEFAULT -1,
"curve_addr"  INTEGER DEFAULT 25089,
"curve_type"  INTEGER DEFAULT 0,
"calc_method"  INTEGER DEFAULT 0,
"device_id"  INTEGER DEFAULT -1,
"ai_start_point"  INTEGER DEFAULT -1,
"realtime_ai"  INTEGER DEFAULT -1,
"unit"  INTEGER DEFAULT 0
);
CREATE TABLE "sa0055_global_para" (
"para_id"  INTEGER NOT NULL,
"para_name"  TEXT(256) NOT NULL,
"para_value"  TEXT(256) NOT NULL,
"para_type"  INTEGER NOT NULL DEFAULT 0,
"int_low_limit"  INTEGER DEFAULT 0,
"int_high_limit"  INTEGER DEFAULT 0,
"double_low_limit"  REAL DEFAULT 0,
"double_high_limit"  REAL DEFAULT 0,
PRIMARY KEY ("para_id" ASC)
);
INSERT INTO "sa0055_global_para" VALUES(1,'遥控密码校验','1',3,0,0,0.0,0.0);
INSERT INTO "sa0055_global_para" VALUES(2,'遥调密码校验','1',3,0,0,0.0,0.0);
INSERT INTO "sa0055_global_para" VALUES(3,'数据推送间隔(秒)','3',1,0,65535,0.0,0.0);
INSERT INTO "sa0055_global_para" VALUES(4,'遥控界面超时(秒)','15',1,0,65535,0.0,0.0);
INSERT INTO "sa0055_global_para" VALUES(5,'遥调界面超时(秒)','15',1,0,65535,0.0,0.0);
INSERT INTO "sa0055_global_para" VALUES(6,'历史数据保存天数','731',1,0,65535,0.0,0.0);
CREATE TABLE "sa0055_formula_type_enum" (
"id"  INTEGER,
"name"  TEXT,
PRIMARY KEY ("id")
);
INSERT INTO "sa0055_formula_type_enum" VALUES(0,'布尔型(DI)');
INSERT INTO "sa0055_formula_type_enum" VALUES(1,'数值型(AI)');
CREATE TABLE "sa0055_do_type_enum" (
"id"  INTEGER,
"name"  TEXT(256),
PRIMARY KEY ("id")
);
INSERT INTO "sa0055_do_type_enum" VALUES(0,'选择执行');
INSERT INTO "sa0055_do_type_enum" VALUES(1,'立即执行');
CREATE TABLE "sa0055_do_step_type_enum" (
"id"  INTEGER,
"name"  TEXT(256),
PRIMARY KEY ("id")
);
INSERT INTO "sa0055_do_step_type_enum" VALUES(0,'降');
INSERT INTO "sa0055_do_step_type_enum" VALUES(1,'升');
CREATE TABLE "sa0055_do_qualifier_enum" (
"id"  INTEGER,
"name"  TEXT(256),
PRIMARY KEY ("id")
);
INSERT INTO "sa0055_do_qualifier_enum" VALUES(0,'未指定');
INSERT INTO "sa0055_do_qualifier_enum" VALUES(1,'短脉冲');
INSERT INTO "sa0055_do_qualifier_enum" VALUES(2,'长脉冲');
INSERT INTO "sa0055_do_qualifier_enum" VALUES(3,'持续');
CREATE TABLE "sa0055_cos_display_enum" (
"id"  INTEGER,
"name"  TEXT,
PRIMARY KEY ("id")
);
INSERT INTO "sa0055_cos_display_enum" VALUES(0,'分开|闭合');
INSERT INTO "sa0055_cos_display_enum" VALUES(1,'动作|复归');
INSERT INTO "sa0055_cos_display_enum" VALUES(2,'分闸|合闸');
INSERT INTO "sa0055_cos_display_enum" VALUES(3,'正常|中断');
CREATE TABLE "sa0055_bool_enum" (
"id"  INTEGER,
"name"  TEXT(256),
PRIMARY KEY ("id")
);
INSERT INTO "sa0055_bool_enum" VALUES(0,'否');
INSERT INTO "sa0055_bool_enum" VALUES(1,'是');
CREATE TABLE "sa0055_alarm_type_enum" (
"id"  INTEGER NOT NULL,
"name"  TEXT,
PRIMARY KEY ("id")
);
INSERT INTO "sa0055_alarm_type_enum" VALUES(0,'事故信息');
INSERT INTO "sa0055_alarm_type_enum" VALUES(1,'故障信息');
INSERT INTO "sa0055_alarm_type_enum" VALUES(2,'变位信息');
INSERT INTO "sa0055_alarm_type_enum" VALUES(3,'越限信息');
INSERT INTO "sa0055_alarm_type_enum" VALUES(4,'告知信息');
CREATE TABLE "config_ai_map" (
"id"  INTEGER NOT NULL DEFAULT -1,
"name"  TEXT(256) NOT NULL DEFAULT '',
"valid"  INTEGER NOT NULL DEFAULT 0,
PRIMARY KEY ("id")
);
CREATE TABLE "config_ao_map" (
"id"  INTEGER NOT NULL DEFAULT -1,
"name"  TEXT(256) NOT NULL DEFAULT '',
"valid"  INTEGER NOT NULL DEFAULT 0,
PRIMARY KEY ("id")
);
CREATE TABLE "config_ci_map" (
"id"  INTEGER NOT NULL DEFAULT -1,
"name"  TEXT(256) NOT NULL DEFAULT '',
"valid"  INTEGER NOT NULL DEFAULT 0,
PRIMARY KEY ("id")
);
CREATE TABLE "config_curve_map" (
"id"  INTEGER NOT NULL DEFAULT -1,
"name"  TEXT(256) NOT NULL DEFAULT '',
"valid"  INTEGER NOT NULL DEFAULT 0,
PRIMARY KEY ("id")
);
CREATE TABLE "config_device_map" (
"id"  INTEGER NOT NULL DEFAULT -1,
"name"  TEXT(256) NOT NULL DEFAULT '',
"valid"  INTEGER NOT NULL DEFAULT 0,
PRIMARY KEY ("id")
);
CREATE TABLE "config_di_map" (
"id"  INTEGER NOT NULL DEFAULT -1,
"name"  TEXT(256) NOT NULL DEFAULT '',
"valid"  INTEGER NOT NULL DEFAULT 0,
PRIMARY KEY ("id")
);
CREATE TABLE "config_do_map" (
"id"  INTEGER NOT NULL DEFAULT -1,
"name"  TEXT(256) NOT NULL DEFAULT '',
"valid"  INTEGER NOT NULL DEFAULT 0,
PRIMARY KEY ("id")
);
CREATE TABLE "config_formula_map" (
"id"  INTEGER NOT NULL DEFAULT -1,
"name"  TEXT NOT NULL DEFAULT '',
"valid"  INTEGER NOT NULL DEFAULT 0,
PRIMARY KEY ("id")
);
CREATE TABLE "config_integral_map" (
"id"  INTEGER DEFAULT -1,
"name"  TEXT DEFAULT '',
"valid"  INTEGER DEFAULT 0,
PRIMARY KEY ("id")
);
CREATE TABLE "sa0055_data_type_enum" (
"id"  INTEGER NOT NULL,
"name"  TEXT NOT NULL,
PRIMARY KEY ("id")
);
INSERT INTO "sa0055_data_type_enum" VALUES(0,'遥信(DI)');
INSERT INTO "sa0055_data_type_enum" VALUES(1,'遥测(AI)');
INSERT INTO "sa0055_data_type_enum" VALUES(2,'遥控(DO)');
INSERT INTO "sa0055_data_type_enum" VALUES(3,'遥调(AO)');
INSERT INTO "sa0055_data_type_enum" VALUES(4,'累计量(CI)');
CREATE TABLE "config_calc_var" (
"seq_num"  INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
"calc_id"  INTEGER,
"formula_id"  INTEGER,
"formula_type"  INTEGER
);
CREATE TABLE "sa0055_alarm_cause_enum" (
"id"  INTEGER,
"name"  TEXT,
"sub_cause"  TEXT,
"data_type"  INTEGER,
PRIMARY KEY ("id" ASC)
);
INSERT INTO "sa0055_alarm_cause_enum" VALUES(0,'遥信变位(COS)','1:分开|2:闭合',0);
INSERT INTO "sa0055_alarm_cause_enum" VALUES(1,'事件记录(SOE)','1:分开|2:闭合',0);
INSERT INTO "sa0055_alarm_cause_enum" VALUES(2,'遥测(AI)越限','1:越上限|2:越下限|3:越上上限|4:越下下限|5:遥测恢复',1);
INSERT INTO "sa0055_alarm_cause_enum" VALUES(3,'遥信(DI)人工置数','1:置数|2:取消置数',0);
INSERT INTO "sa0055_alarm_cause_enum" VALUES(4,'遥测(AI)人工置数','1:置数|2:取消置数',1);
INSERT INTO "sa0055_alarm_cause_enum" VALUES(5,'遥信(DI)挂牌','1:挂牌|2:摘牌',0);
INSERT INTO "sa0055_alarm_cause_enum" VALUES(6,'遥测(AI)挂牌','1:挂牌|2:摘牌',1);
INSERT INTO "sa0055_alarm_cause_enum" VALUES(7,'装置挂牌','1:挂牌|2:摘牌',5);
INSERT INTO "sa0055_alarm_cause_enum" VALUES(8,'遥控(DO)操作','1:遥控冲突|2:遥控执行失败|3:遥控执行成功|4:遥控执行超时|5:遥控选择失败|6:遥控选择成功|7:遥控选择超时',2);
INSERT INTO "sa0055_alarm_cause_enum" VALUES(9,'遥调(AO)操作','1:遥调冲突|2:遥调执行失败|3:遥调执行成功|4:遥调执行超时',3);
INSERT INTO "sa0055_alarm_cause_enum" VALUES(10,'系统日志','1:用户登陆|2:用户退出|3:修改配置|4:重启服务器',-1);
CREATE TABLE "sa0055_server_run_mode_enum" (
"id"  INTEGER NOT NULL,
"name"  TEXT(256),
PRIMARY KEY ("id")
);
INSERT INTO "sa0055_server_run_mode_enum" VALUES(0,'单服务器');
INSERT INTO "sa0055_server_run_mode_enum" VALUES(1,'双服务器');
CREATE TABLE "sa0055_network_mode_enum" (
"id"  INTEGER NOT NULL,
"name"  TEXT(256),
PRIMARY KEY ("id")
);
INSERT INTO "sa0055_network_mode_enum" VALUES(0,'单网');
INSERT INTO "sa0055_network_mode_enum" VALUES(1,'双网');
CREATE TABLE "config_device_para" (
"seq_num"  INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
"device_id"  INTEGER NOT NULL,
"device_run_mode"  INTEGER DEFAULT 0,
"network_mode"  INTEGER DEFAULT 0,
"device1_ip_a"  TEXT(16) DEFAULT '0.0.0.0',
"device1_ip_b"  TEXT(16) DEFAULT '0.0.0.0',
"device2_ip_a"  TEXT(16) DEFAULT '0.0.0.0',
"device2_ip_b"  TEXT(16) DEFAULT '0.0.0.0'
);
CREATE TABLE "config_server_para" (
"server_run_mode"  INTEGER NOT NULL DEFAULT 1,
"network_mode"  INTEGER NOT NULL DEFAULT 1,
"server1_ip_a"  TEXT(16) NOT NULL DEFAULT '0.0.0.0',
"server1_ip_b"  TEXT(16) NOT NULL DEFAULT '0.0.0.0',
"server2_ip_a"  TEXT(16) NOT NULL DEFAULT '0.0.0.0',
"server2_ip_b"  TEXT(16) NOT NULL DEFAULT '0.0.0.0',
"virtual_ip_a"  TEXT(16) NOT NULL DEFAULT '0.0.0.0',
"virtual_ip_b"  TEXT(16) NOT NULL DEFAULT '0.0.0.0',
PRIMARY KEY ("virtual_ip_a")
);
DELETE FROM sqlite_sequence;
INSERT INTO "sqlite_sequence" VALUES('config_device_para',NULL);
COMMIT;
