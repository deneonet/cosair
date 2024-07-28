#pragma once

#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "cosair/debug/instrumentor.h"
#include "cosair/debug/log.h"

#ifdef CR_PLATFORM_WINDOWS
#include <Windows.h>
#endif