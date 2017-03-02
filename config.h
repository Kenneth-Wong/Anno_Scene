#pragma once

#ifndef CONFIG_H_
#define CONFIG_H_


#define PLACES  "LOCK\\places205.txt"
#define ATTRIBUTE "LOCK\\SUN_attribute.txt"

#define TRAIN14_CLASS "LOCK\\PredictData\\Class\\train2014_label_mini.txt"
#define VAL14_CLASS "LOCK\\PredictData\\Class\\val2014_label_mini.txt"
#define TEST15_CLASS "LOCK\\PredictData\\Class\\test2015_label_mini.txt"
#define TRAIN14_ATTR "LOCK\\PredictData\\Attribute\\train2014_label_mini.txt"
#define VAL14_ATTR "LOCK\\PredictData\\Attribute\\val2014_label_mini.txt"
#define TEST15_ATTR "LOCK\\PredictData\\Attribute\\test2015_label_mini.txt"

#define SAMPLEROOT "LOCK\\samples\\"
#define SAVEROOT "Annotation\\"


#define TRAIN14 "train2014"
#define VAL14 "val2014"
#define TEST15 "test2015"


#define TRAIN 0
#define VAL 1
#define TEST 2

#define TRAIN_NUM 82783
#define VAL_NUM 40504
#define TEST_NUM 81434

#define NOSCENE 301
#define NOLABEL 302


#endif