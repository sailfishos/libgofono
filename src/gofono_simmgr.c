/*
 * Copyright (C) 2014-2016 Jolla Ltd.
 * Contact: Slava Monich <slava.monich@jolla.com>
 *
 * You may use this file under the terms of BSD license as follows:
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   1. Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *   3. Neither the name of the Jolla Ltd nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "gofono_simmgr.h"
#include "gofono_modem_p.h"
#include "gofono_names.h"
#include "gofono_log.h"

/* Generated headers */
#define OFONO_OBJECT_PROXY OrgOfonoSimManager
#include "org.ofono.SimManager.h"
#include "gofono_modemintf_p.h"

/* Object definition */
struct ofono_simmgr_priv {
    const char* name;
    char* imsi;
    char* mcc;
    char* mnc;
    char* spn;
};

typedef OfonoModemInterfaceClass OfonoSimMgrClass;
G_DEFINE_TYPE(OfonoSimMgr, ofono_simmgr, OFONO_TYPE_MODEM_INTERFACE)

#define SIMMGR_SIGNAL_PRESENT_CHANGED_NAME "present-changed"
#define SIMMGR_SIGNAL_IMSI_CHANGED_NAME    "imsi-changed"
#define SIMMGR_SIGNAL_MCC_CHANGED_NAME     "mcc-changed"
#define SIMMGR_SIGNAL_MNC_CHANGED_NAME     "mnc-changed"
#define SIMMGR_SIGNAL_SPN_CHANGED_NAME     "spn-changed"

/*==========================================================================*
 * Implementation
 *==========================================================================*/

#if GUTIL_LOG_DEBUG
static
gboolean
ofono_simmgr_initialized(
    OfonoObject* obj)
{
    OfonoSimMgr* self = OFONO_SIMMGR(obj);
    OfonoSimMgrPriv* priv = self->priv;
    GDEBUG("%s: SIM %spresent", priv->name, self->present ? "" : "not ");
    GDEBUG("%s: IMSI %s", priv->name, self->imsi);
    return OFONO_OBJECT_CLASS(ofono_simmgr_parent_class)->fn_initialized(obj);
}
#endif /* GUTIL_LOG_DEBUG */

/*==========================================================================*
 * API
 *==========================================================================*/

OfonoSimMgr*
ofono_simmgr_new(
    const char* path)
{
    const char* ifname = OFONO_SIMMGR_INTERFACE_NAME;
    OfonoModem* modem = ofono_modem_new(path);
    OfonoSimMgr* simmgr;
    OfonoModemInterface* intf = ofono_modem_get_interface(modem, ifname);
    if (G_TYPE_CHECK_INSTANCE_TYPE(intf, OFONO_TYPE_SIMMGR)) {
        /* Reuse the existing object */
        simmgr = ofono_simmgr_ref(OFONO_SIMMGR(intf));
    } else {
        simmgr = g_object_new(OFONO_TYPE_SIMMGR, NULL);
        intf = &simmgr->intf;
        GVERBOSE_("%s", path);
        ofono_modem_interface_initialize(intf, ifname, path);
        ofono_modem_set_interface(modem, intf);
        simmgr->priv->name = ofono_object_name(&intf->object);
        GASSERT(intf->modem == modem);
    }
    ofono_modem_unref(modem);
    return simmgr;
}

OfonoSimMgr*
ofono_simmgr_ref(
    OfonoSimMgr* self)
{
    if (G_LIKELY(self)) {
        g_object_ref(OFONO_SIMMGR(self));
        return self;
    } else {
        return NULL;
    }
}

void
ofono_simmgr_unref(
    OfonoSimMgr* self)
{
    if (G_LIKELY(self)) {
        g_object_unref(OFONO_SIMMGR(self));
    }
}

gulong
ofono_simmgr_add_property_changed_handler(
    OfonoSimMgr* self,
    OfonoSimMgrPropertyHandler fn,
    const char* name,
    void* arg)
{
    return G_LIKELY(self) ? ofono_object_add_property_changed_handler(
        &self->intf.object, (OfonoObjectPropertyHandler)fn, name, arg) :  0;
}

gulong
ofono_simmgr_add_valid_changed_handler(
    OfonoSimMgr* self,
    OfonoSimMgrHandler fn,
    void* arg)
{
    return G_LIKELY(self) ? ofono_object_add_valid_changed_handler(
        &self->intf.object, (OfonoObjectHandler)fn, arg) : 0;
}

gulong
ofono_simmgr_add_imsi_changed_handler(
    OfonoSimMgr* self,
    OfonoSimMgrHandler fn,
    void* arg)
{
    return (G_LIKELY(self) && G_LIKELY(fn)) ? g_signal_connect(self,
        SIMMGR_SIGNAL_IMSI_CHANGED_NAME, G_CALLBACK(fn), arg) : 0;
}

gulong
ofono_simmgr_add_mcc_changed_handler(
    OfonoSimMgr* self,
    OfonoSimMgrHandler fn,
    void* arg)
{
    return (G_LIKELY(self) && G_LIKELY(fn)) ? g_signal_connect(self,
        SIMMGR_SIGNAL_MCC_CHANGED_NAME, G_CALLBACK(fn), arg) : 0;
}

gulong
ofono_simmgr_add_mnc_changed_handler(
    OfonoSimMgr* self,
    OfonoSimMgrHandler fn,
    void* arg)
{
    return (G_LIKELY(self) && G_LIKELY(fn)) ? g_signal_connect(self,
        SIMMGR_SIGNAL_MNC_CHANGED_NAME, G_CALLBACK(fn), arg) : 0;
}

gulong
ofono_simmgr_add_spn_changed_handler(
    OfonoSimMgr* self,
    OfonoSimMgrHandler fn,
    void* arg)
{
    return (G_LIKELY(self) && G_LIKELY(fn)) ? g_signal_connect(self,
        SIMMGR_SIGNAL_SPN_CHANGED_NAME, G_CALLBACK(fn), arg) : 0;
}

gulong
ofono_simmgr_add_present_changed_handler(
    OfonoSimMgr* self,
    OfonoSimMgrHandler fn,
    void* arg)
{
    return (G_LIKELY(self) && G_LIKELY(fn)) ? g_signal_connect(self,
        SIMMGR_SIGNAL_PRESENT_CHANGED_NAME, G_CALLBACK(fn), arg) : 0;
}

void
ofono_simmgr_remove_handler(
    OfonoSimMgr* self,
    gulong id)
{
    if (G_LIKELY(self) && G_LIKELY(id)) {
        g_signal_handler_disconnect(self, id);
    }
}

/*==========================================================================*
 * Properties
 *==========================================================================*/

#define SIMMGR_DEFINE_PROPERTY_BOOL(NAME,var) \
    OFONO_OBJECT_DEFINE_PROPERTY_BOOL(SIMMGR,NAME,OfonoSimMgr,var)

#define SIMMGR_DEFINE_PROPERTY_STRING(NAME,var) \
    OFONO_OBJECT_DEFINE_PROPERTY_STRING(SIMMGR,simmgr,NAME,OfonoSimMgr,var)

static
void*
ofono_simmgr_property_priv(
    OfonoObject* object,
    const OfonoObjectProperty* prop)
{
    return OFONO_SIMMGR(object)->priv;
}

/*==========================================================================*
 * Internals
 *==========================================================================*/

/**
 * Per instance initializer
 */
static
void
ofono_simmgr_init(
    OfonoSimMgr* self)
{
    self->priv = G_TYPE_INSTANCE_GET_PRIVATE(self,
        OFONO_TYPE_SIMMGR, OfonoSimMgrPriv);
}

/**
 * Final stage of deinitialization
 */
static
void
ofono_simmgr_finalize(
    GObject* object)
{
    OfonoSimMgr* self = OFONO_SIMMGR(object);
    OfonoSimMgrPriv* priv = self->priv;
    g_free(priv->imsi);
    g_free(priv->mcc);
    g_free(priv->mnc);
    g_free(priv->spn);
    G_OBJECT_CLASS(ofono_simmgr_parent_class)->finalize(object);
}

/**
 * Per class initializer
 */
static
void
ofono_simmgr_class_init(
    OfonoSimMgrClass* klass)
{
    static OfonoObjectProperty ofono_simmgr_properties[] = {
        SIMMGR_DEFINE_PROPERTY_BOOL(PRESENT,present),
        SIMMGR_DEFINE_PROPERTY_STRING(IMSI,imsi),
        SIMMGR_DEFINE_PROPERTY_STRING(MCC,mcc),
        SIMMGR_DEFINE_PROPERTY_STRING(MNC,mnc),
        SIMMGR_DEFINE_PROPERTY_STRING(SPN,spn)
    };

    OfonoObjectClass* ofono = &klass->object;
    G_OBJECT_CLASS(klass)->finalize = ofono_simmgr_finalize;
    g_type_class_add_private(klass, sizeof(OfonoSimMgrPriv));
#if GUTIL_LOG_DEBUG
    ofono->fn_initialized = ofono_simmgr_initialized;
#endif /* GUTIL_LOG_DEBUG */
    ofono->properties = ofono_simmgr_properties;
    ofono->nproperties = G_N_ELEMENTS(ofono_simmgr_properties);
    OFONO_OBJECT_CLASS_SET_PROXY_CALLBACKS(ofono, org_ofono_sim_manager);
    ofono_class_initialize(ofono);
}

/*
 * Local Variables:
 * mode: C
 * c-basic-offset: 4
 * indent-tabs-mode: nil
 * End:
 */
