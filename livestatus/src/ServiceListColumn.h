// +------------------------------------------------------------------+
// |             ____ _               _        __  __ _  __           |
// |            / ___| |__   ___  ___| | __   |  \/  | |/ /           |
// |           | |   | '_ \ / _ \/ __| |/ /   | |\/| | ' /            |
// |           | |___| | | |  __/ (__|   <    | |  | | . \            |
// |            \____|_| |_|\___|\___|_|\_\___|_|  |_|_|\_\           |
// |                                                                  |
// | Copyright Mathias Kettner 2014             mk@mathias-kettner.de |
// +------------------------------------------------------------------+
//
// This file is part of Check_MK.
// The official homepage is at http://mathias-kettner.de/check_mk.
//
// check_mk is free software;  you can redistribute it and/or modify it
// under the  terms of the  GNU General Public License  as published by
// the Free Software Foundation in version 2.  check_mk is  distributed
// in the hope that it will be useful, but WITHOUT ANY WARRANTY;  with-
// out even the implied warranty of  MERCHANTABILITY  or  FITNESS FOR A
// PARTICULAR PURPOSE. See the  GNU General Public License for more de-
// tails. You should have  received  a copy of the  GNU  General Public
// License along with GNU Make; see the file  COPYING.  If  not,  write
// to the Free Software Foundation, Inc., 51 Franklin St,  Fifth Floor,
// Boston, MA 02110-1301 USA.

#ifndef ServiceListColumn_h
#define ServiceListColumn_h

#include "config.h"  // IWYU pragma: keep
#include <memory>
#include <string>
#include "Column.h"
#include "opids.h"
class Filter;
class MonitoringCore;
class Row;
class RowRenderer;

#ifdef CMC
#include "Host.h"
#include "cmc.h"
#else
#include "nagios.h"
#endif

class ServiceListColumn : public Column {
public:
    ServiceListColumn(const std::string &name, const std::string &description,
                      int indirect_offset, int extra_offset,
                      int extra_extra_offset, int offset, MonitoringCore *mc,
                      bool hostname_required, int info_depth)
        : Column(name, description, indirect_offset, extra_offset,
                 extra_extra_offset, offset)
        , _mc(mc)
        , _hostname_required(hostname_required)
        , _info_depth(info_depth) {}
    ColumnType type() const override { return ColumnType::list; };
    void output(Row row, RowRenderer &r,
                const contact *auth_user) const override;
    std::unique_ptr<Filter> createFilter(
        RelationalOperator relOp, const std::string &value) const override;

#ifdef CMC
    using service_list = const Host::services_t *;
#else
    using service_list = servicesmember *;
#endif
    service_list getMembers(Row row) const;

private:
    MonitoringCore *_mc;
    bool _hostname_required;
    int _info_depth;

#ifndef CMC
    int inCustomTimeperiod(service *svc, const char *varname) const;
#endif
};

#endif  // ServiceListColumn_h
