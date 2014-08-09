/* Copyright (C) 2014 J.F.Dockes
 *       This program is free software; you can redistribute it and/or modify
 *       it under the terms of the GNU General Public License as published by
 *       the Free Software Foundation; either version 2 of the License, or
 *       (at your option) any later version.
 *
 *       This program is distributed in the hope that it will be useful,
 *       but WITHOUT ANY WARRANTY; without even the implied warranty of
 *       MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *       GNU General Public License for more details.
 *
 *       You should have received a copy of the GNU General Public License
 *       along with this program; if not, write to the
 *       Free Software Foundation, Inc.,
 *       59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */
#ifndef _SERVICE_H_X_INCLUDED_
#define _SERVICE_H_X_INCLUDED_

#include <string>
#include <functional>
#include <unordered_map>

#include <upnp/upnp.h>

#include "libupnpp/soaphelp.hxx"
#include "libupnpp/upnpp_p.hxx"
#include "libupnpp/description.hxx"

namespace UPnPClient {

typedef 
std::function<void (const std::unordered_map<std::string, std::string>&)> 
evtCBFunc;

class Service {
public:
    /** Construct by copying data from device and service objects.
     *
     * Mostly used by the discovery service. 
     */
    Service(const UPnPDeviceDesc& device,
            const UPnPServiceDesc& service)
        : m_actionURL(caturl(device.URLBase, service.controlURL)),
          m_eventURL(caturl(device.URLBase, service.eventSubURL)),
          m_serviceType(service.serviceType),
          m_deviceId(device.UDN),
          m_friendlyName(device.friendlyName),
          m_manufacturer(device.manufacturer),
          m_modelName(device.modelName)
    { 
        initEvents();
        subscribe();
    }

    /** An empty one */
    Service() {}

    virtual ~Service() {}

    /** Retrieve my root device "friendly name". */
    std::string getFriendlyName() const {return m_friendlyName;}

    /** Return my root device id */
    std::string getDeviceId() const {return m_deviceId;}

    virtual int runAction(const SoapEncodeInput& args, SoapDecodeOutput& data);

protected:

    /** Registered callbacks for our derived classes */
    static std::unordered_map<std::string, evtCBFunc> o_calls;

    /** Used by derived class to register its callback method */
    void registerCallback(evtCBFunc c);

    std::string m_actionURL;
    std::string m_eventURL;
    std::string m_serviceType;
    std::string m_deviceId;
    std::string m_friendlyName;
    std::string m_manufacturer;
    std::string m_modelName;

private:
    /** Only actually does something on the first call, to register our
        library callback */
    static bool initEvents();
    /** The event callback given to libupnp */
    static int srvCB(Upnp_EventType et, void* vevp, void*);
    /* Tell the UPnP device that we want to receive its events */
    virtual bool subscribe();

    Upnp_SID    m_SID; /* Subscription Id */
};

extern Service *service_factory(const std::string& servicetype,
                                const UPnPDeviceDesc& device,
                                const UPnPServiceDesc& service);

} // namespace UPnPClient

#endif /* _SERVICE_H_X_INCLUDED_ */