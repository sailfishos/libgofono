/*
 * Copyright (C) 2016-2020 Jolla Ltd.
 * Copyright (C) 2016-2020 Slava Monich <slava.monich@jolla.com>
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
 *   3. Neither the names of the copyright holders nor the names of its
 *      contributors may be used to endorse or promote products derived
 *      from this software without specific prior written permission.
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

#ifndef GOFONO_MANAGER_PROXY_H
#define GOFONO_MANAGER_PROXY_H

#include "gofono_types.h"

typedef struct ofono_manager_proxy_priv OfonoManagerProxyPriv;

typedef struct ofono_manager_prixy {
    GObject object;
    OfonoManagerProxyPriv* priv;
    GPtrArray* modem_paths;
    gboolean valid;
} OfonoManagerProxy;

typedef
void
(*OfonoManagerProxyHandler)(
    OfonoManagerProxy* proxy,
    void* arg);

typedef
void
(*OfonoManagerProxyModemHandler)(
    OfonoManagerProxy* proxy,
    const char* modem,
    void* arg);

OfonoManagerProxy*
ofono_manager_proxy_new(void)
    G_GNUC_INTERNAL;

gboolean
ofono_manager_proxy_has_modem(
    OfonoManagerProxy* proxy,
    const char* path)
    G_GNUC_INTERNAL;

gulong
ofono_manager_proxy_add_valid_changed_handler(
    OfonoManagerProxy* proxy,
    OfonoManagerProxyHandler fn,
    void* arg)
    G_GNUC_INTERNAL;

gulong
ofono_manager_proxy_add_modem_added_handler(
    OfonoManagerProxy* proxy,
    OfonoManagerProxyModemHandler fn,
    void* arg)
    G_GNUC_INTERNAL;

gulong
ofono_manager_proxy_add_modem_removed_handler(
    OfonoManagerProxy* proxy,
    OfonoManagerProxyModemHandler fn,
    void* arg)
    G_GNUC_INTERNAL;

#endif /* GOFONO_MANAGER_PROXY_H */

/*
 * Local Variables:
 * mode: C
 * c-basic-offset: 4
 * indent-tabs-mode: nil
 * End:
 */
