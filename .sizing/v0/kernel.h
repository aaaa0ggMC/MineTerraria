#ifndef KERNEL_H_INCLUDED
#define KERNEL_H_INCLUDED

//can use output
#define MODE_DEBUG

#define OUT_LOGS

#include <SFML/Graphics.hpp>
#include <iostream>
#include <windows.h>
#include <pthread.h>
#include <string>
#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <malloc.h>
#include <math.h>
#include <io.h>
#include "debugIO.h"
#include "resource.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "zipTool/unzip.h"
#include "CClock.h"
#include "ctool.h"

///JSON
#define ER_JSON_PARSE 1


///Statuses
#define EXECUTE_SUC 0
#define EXECUTE_FAI -1

#endif // KERNEL_H_INCLUDED
