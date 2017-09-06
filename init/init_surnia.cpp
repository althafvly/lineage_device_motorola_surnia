/*
   Copyright (c) 2014, The Linux Foundation. All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.
    * Neither the name of The Linux Foundation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
   WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
   ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
   BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
   BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
   OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
   IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <android-base/properties.h>
#include <stdlib.h>
#include <stdio.h>
#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>

#include "vendor_init.h"
#include "property_service.h"
#include "log.h"
namespace android {
namespace init {
void gsm_properties(bool msim);
void cdma_properties();

void property_override(char const prop[], char const value[])
{
    prop_info *pi;

    pi = (prop_info*) __system_property_find(prop);
    if (pi)
        __system_property_update(pi, value, strlen(value));
    else
        __system_property_add(prop, strlen(prop), value, strlen(value));
}

void vendor_load_properties()
{
    std::string carrier;
    std::string device;
    std::string fsg;
    std::string platform;
    std::string radio;

    platform = android::base::GetProperty("ro.board.platform","platform");
    if (platform != ANDROID_TARGET)
        return;

    radio = android::base::GetProperty("ro.boot.radio","radio");
    carrier = android::base::GetProperty("ro.boot.carrier","carrier");
    fsg = android::base::GetProperty("ro.boot.fsg-id","fsg");

    if (radio == "0x2") {
        /* XT1529 */
        cdma_properties();
        property_override("ro.product.model", "XT1529");
        property_override("ro.product.device", "surnia_cdma");
        property_override("ro.build.product", "surnia_cdma");
        property_override("ro.build.description", "surnia_verizon-user 5.1 LPI23.29-17.5 5 release-keys");
        property_override("ro.build.fingerprint", "motorola/surnia_verizon/surnia_cdma:5.1/LPI23.29-17.5/5:user/release-keys");
        android::base::SetProperty("ro.mot.build.customerid", "verizon");
        android::base::SetProperty("ro.telephony.get_imsi_from_sim", "true");
        android::base::SetProperty("ro.com.google.clientidbase.am", "android-verizon");
        android::base::SetProperty("ro.com.google.clientidbase.ms", "android-verizon");
        android::base::SetProperty("ro.com.google.clientidbase.yt", "android-verizon");
        android::base::SetProperty("ro.cdma.data_retry_config", "max_retries=infinite,0,0,10000,10000,100000,10000,10000,10000,10000,140000,540000,960000");
    } else if (radio == "0x1") {
        /* XT1527 */
        gsm_properties(false);
        property_override("ro.product.model", "XT1527");
        property_override("ro.product.device", "surnia_umts");
        property_override("ro.build.product", "surnia_umts");
        property_override("ro.build.description", "surnia_retus-user 5.0.2 LXI22.50-24.1 1 release-keys");
        property_override("ro.build.fingerprint", "motorola/surnia_retus/surnia_umts:5.0.2/LXI22.50-24.1/1:user/release-keys");
        android::base::SetProperty("ro.mot.build.customerid", "retus");
    } else if (radio == "0x3") {
        /* XT1526 */
        cdma_properties();
        if (carrier == "sprint") {
            if (fsg == "boost") {
                property_override("ro.build.description", "surnia_boost-user 5.0.2 LXI22.50-14.8 30 release-keys");
                property_override("ro.build.fingerprint", "motorola/surnia_boost/surnia_cdma:5.0.2/LXI22.50-14.8/30:user/release-keys");
	        android::base::SetProperty("ro.cdma.home.operator.numeric", "311870");
	        android::base::SetProperty("ro.cdma.home.operator.alpha", "Boost Mobile");
            } else {
                property_override("ro.build.description", "surnia_sprint-user 5.0.2 LXI22.50-14.8 30 release-keys");
                property_override("ro.build.fingerprint", "motorola/surnia_sprint/surnia_cdma:5.0.2/LXI22.50-14.8/30:user/release-keys");
            }
            android::base::SetProperty("ro.fsg-id", "sprint");
            android::base::SetProperty("ro.carrier", "sprint");
            android::base::SetProperty("ro.mot.build.customerid ","sprint");
        } else {
            property_override("ro.build.description", "surnia_usc-user 5.0.2 LXI22.50-14.8 30 release-keys");
            property_override("ro.build.fingerprint", "motorola/surnia_usc/surnia_cdma:5.0.2/LXI22.50-14.8/30:user/release-keys");
            android::base::SetProperty("ro.mot.build.customerid", "usc");
            android::base::SetProperty("ro.cdma.home.operator.alpha", "U.S. Cellular");
            android::base::SetProperty("ro.cdma.home.operator.numeric", "311580");
            android::base::SetProperty("ro.fsg-id", "usc");
        }
        property_override("ro.product.model", "XT1526");
        property_override("ro.product.device", "surnia_cdma");
        property_override("ro.build.product", "surnia_cdma");
        android::base::SetProperty("persist.radio.0x9e_not_callname","1");
        android::base::SetProperty("persist.radio.lifecalls", "0");
        android::base::SetProperty("persist.radio.lifetimer", "0");
        android::base::SetProperty("persist.radio.multisim.config", "");
        android::base::SetProperty("ro.cdma.international.eri", "2,74,124,125,126,157,158,159,193,194,195,196,197,198,228,229,230,231,232,233,234,235");
        android::base::SetProperty("ro.com.android.dataroaming","false");
    } else if (radio == "0x4") {
        /* XT1524 */
        gsm_properties(false);
        property_override("ro.product.model", "XT1524");
        property_override("ro.product.device", "surnia_umts");
        property_override("ro.build.product", "surnia_umts");
        property_override("ro.build.description", "surnia_reteu-user 5.0.2 LXI22.50-24.1 2 release-keys");
        property_override("ro.build.fingerprint", "motorola/surnia_reteu/surnia_umts:5.0.2/LXI22.50-24.1/2:user/release-keys");
        android::base::SetProperty("ro.mot.build.customerid", "reteuall");
    } else if (radio == "0x6") {
        /* XT1523 */
        gsm_properties(true);
        property_override("ro.product.model", "XT1523");
        property_override("ro.product.device", "surnia_udstv");
        property_override("ro.build.product", "surnia_udstv");
        property_override("ro.build.description", "surnia_retbr_dstv-user 5.0.2 LXI22.50-24.1 3 release-keys");
        property_override("ro.build.fingerprint", "motorola/surnia_retbr_dstv/surnia_udstv:5.0.2/LXI22.50-24.1/3:user/release-keys");
        android::base::SetProperty("ro.mot.build.customerid", "retbr");
    } else if (radio == "0x7") {
        /* XT1521 */
        gsm_properties(true);
        property_override("ro.product.model", "XT1521");
        property_override("ro.product.device", "surnia_uds");
        property_override("ro.build.product", "surnia_uds");
        property_override("ro.build.description", "surnia_retasia_ds-user 5.0.2 LXI22.50-53.1 1 release-keys");
        property_override("ro.build.fingerprint", "motorola/surnia_retasia_ds/surnia_uds:5.0.2/LXI22.50-53.1/1:user/release-keys");
        android::base::SetProperty("ro.mot.build.customerid", "retasiaall");
    } else if (radio == "0x8") {
        /* XT1514 */
        gsm_properties(true);
        property_override("ro.product.model", "XT1514");
        property_override("ro.product.device", "surnia_uds");
        property_override("ro.build.product", "surnia_uds");
        property_override("ro.build.description", "surnia_retbr_ds-user 5.0.2 LXI22.50-24.1 1 release-keys");
        property_override("ro.build.fingerprint", "motorola/surnia_retbr_ds/surnia_uds:5.0.2/LXI22.50-24.1/1:user/release-keys");
        android::base::SetProperty("ro.mot.build.customerid", "retbr");
    }
    device = android::base::GetProperty("ro.product.device","device");
 //   INFO("Found radio id: %s setting build properties for %s device\n", radio.c_str(), device.c_str());
}

void cdma_properties()
{
    android::base::SetProperty("DEVICE_PROVISIONED","1");
    android::base::SetProperty("gsm.sim.operator.iso-country", "US");
    android::base::SetProperty("gsm.operator.iso-country", "US");
    android::base::SetProperty("ril.subscription.types","NV,RUIM");
    android::base::SetProperty("ro.telephony.default_cdma_sub", "0");
    android::base::SetProperty("ro.product.locale.region", "US");
    android::base::SetProperty("ro.telephony.default_network", "8");
    android::base::SetProperty("telephony.lteOnCdmaDevice", "1");
}

void gsm_properties(bool msim)
{
    android::base::SetProperty("telephony.lteOnGsmDevice", "1");
    android::base::SetProperty("ro.telephony.default_network", "9");
    if (msim) {
        android::base::SetProperty("persist.radio.dont_use_dsd", "true");
        android::base::SetProperty("persist.radio.multisim.config", "dsds");
        android::base::SetProperty("persist.radio.plmn_name_cmp", "1");
        android::base::SetProperty("ro.telephony.ril.config", "simactivation");
    } else {
        android::base::SetProperty("persist.radio.multisim.config", "");
    }
}
}  // namespace init
}  // namespace android

