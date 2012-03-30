# *************************************************************
#  
#  Licensed to the Apache Software Foundation (ASF) under one
#  or more contributor license agreements.  See the NOTICE file
#  distributed with this work for additional information
#  regarding copyright ownership.  The ASF licenses this file
#  to you under the Apache License, Version 2.0 (the
#  "License"); you may not use this file except in compliance
#  with the License.  You may obtain a copy of the License at
#  
#    http://www.apache.org/licenses/LICENSE-2.0
#  
#  Unless required by applicable law or agreed to in writing,
#  software distributed under the License is distributed on an
#  "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
#  KIND, either express or implied.  See the License for the
#  specific language governing permissions and limitations
#  under the License.
#  
# *************************************************************
Name: fake-db
Version: 1.0
Release: 0
Summary: This is a dummy package
Group: dummy
License: ALv2
BuildArch: noarch
AutoReqProv: no
%define _tmppath /tmp
#BuildRoot: %{_tmppath}/%{name}-root
Provides: libgnomevfs-2.so.0
Provides: libgconf-2.so.4
Provides: libfreetype.so.6
Provides: /bin/sh
Provides: /bin/basename
Provides: /bin/cat
Provides: /bin/cp
Provides: /bin/gawk
Provides: /bin/grep
Provides: /bin/ln
Provides: /bin/ls
Provides: /bin/mkdir
Provides: /bin/mv
Provides: /bin/pwd
Provides: /bin/rm
Provides: /bin/sed
Provides: /bin/sort
Provides: /bin/touch
Provides: /usr/bin/cut
Provides: /usr/bin/dirname
Provides: /usr/bin/expr
Provides: /usr/bin/find
Provides: /usr/bin/tail
Provides: /usr/bin/tr
Provides: /usr/bin/wc
%description
a dummy package
%files
