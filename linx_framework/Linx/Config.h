#pragma once

#define Debug	LOG_DEBUG
#define Info	LOG_INFO
#define Warn	LOG_WARN
#define Error	LOG_ERROR
#define Fatal	LOG_FATAL

#define LOG_OPEN			true
#define LOG_ENABLE			true && LOG_OPEN
#define LOG_FILE			"test.%Y-%m-%d_%H-%M-%S.log"
#define LOG_LEVEL			ELogLevel::LevelInfo
#define LOG_SPLIT_SECONDS	0
#define LOG_SPLIT_MINUTES	0
#define LOG_SPLIT_HOURS		0
#define LOG_SPLIT_DAYS		0
#define LOG_SPLIT_SIZE		0
#define LOG_SPLIT_SIZE_K	0
#define LOG_SPLIT_SIZE_M	0
