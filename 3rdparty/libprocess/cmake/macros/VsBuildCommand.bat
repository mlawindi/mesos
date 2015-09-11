:: Licensed to the Apache Software Foundation (ASF) under one
:: or more contributor license agreements.  See the NOTICE file
:: distributed with this work for additional information
:: regarding copyright ownership.  The ASF licenses this file
:: to you under the Apache License, Version 2.0 (the
:: "License"); you may not use this file except in compliance
:: with the License.  You may obtain a copy of the License at
::
::     http://www.apache.org/licenses/LICENSE-2.0
::
:: Unless required by applicable law or agreed to in writing, software
:: distributed under the License is distributed on an "AS IS" BASIS,
:: WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
:: See the License for the specific language governing permissions and
:: limitations under the License.

@echo off

:: Path to the solution (.sln) file being built.
set SOLUTION_FILE=%~1
:: List of project names delimited by '#'.
set PROJECTS=%~2
set CONFIGURATION=Release
set PLATFORM=Win32
setlocal EnableDelayedExpansion
setlocal EnableExtensions

:: Get Visual Studio install directory.
set KEY_NAME=HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\VisualStudio\14.0
set VALUE_NAME=InstallDir

for /F "skip=2 tokens=1,2*" %%A in ('REG QUERY %KEY_NAME% /v %VALUE_NAME% 2^>nul') do (
  set MSVC_DIR=%%C..\..
)

:: Check if Visual Studio 2015 is installed.
if defined MSVC_DIR (
  set SOLUTION_VER=12.00
) else (
  echo "No compiler : Microsoft Visual Studio (2015) is not installed."
  exit /b 1
)

call "%MSVC_DIR%\VC\vcvarsall.bat" x86

FINDSTR ^
    /C:"Microsoft Visual Studio Solution File, Format Version %SOLUTION_VER%" ^
    %SOLUTION_FILE:/=\%
if %errorlevel% neq 0 (
  devenv /upgrade %SOLUTION_FILE%
)

if not "%PROJECTS%" == "" (
  set PROJECTS_TARGET=/t:%PROJECTS:#=;%
)

msbuild ^
    %SOLUTION_FILE% %PROJECTS_TARGET% ^
    /p:Configuration=%CONFIGURATION%;Platform=%PLATFORM%

:end
exit /b