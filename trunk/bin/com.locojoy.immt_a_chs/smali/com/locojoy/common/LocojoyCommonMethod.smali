.class public Lcom/locojoy/common/LocojoyCommonMethod;
.super Ljava/lang/Object;
.source "LocojoyCommonMethod.java"


# static fields
.field public static assets:Landroid/content/res/AssetManager;

.field public static final empty:[Ljava/lang/String;

.field private static packageManger:Landroid/content/pm/PackageManager;

.field private static s_activity:Landroid/app/Activity;

.field private static wifi:Landroid/net/wifi/WifiManager;


# direct methods
.method static constructor <clinit>()V
    .locals 1

    .prologue
    const/4 v0, 0x0

    .line 29
    sput-object v0, Lcom/locojoy/common/LocojoyCommonMethod;->packageManger:Landroid/content/pm/PackageManager;

    .line 30
    sput-object v0, Lcom/locojoy/common/LocojoyCommonMethod;->s_activity:Landroid/app/Activity;

    .line 131
    const/4 v0, 0x0

    new-array v0, v0, [Ljava/lang/String;

    sput-object v0, Lcom/locojoy/common/LocojoyCommonMethod;->empty:[Ljava/lang/String;

    return-void
.end method

.method public constructor <init>()V
    .locals 0

    .prologue
    .line 25
    invoke-direct {p0}, Ljava/lang/Object;-><init>()V

    return-void
.end method

.method public static forbidLockScreen(Landroid/content/Context;)V
    .locals 2
    .parameter "context"

    .prologue
    const/16 v1, 0x80

    .line 240
    check-cast p0, Landroid/app/Activity;

    .end local p0
    invoke-virtual {p0}, Landroid/app/Activity;->getWindow()Landroid/view/Window;

    move-result-object v0

    invoke-virtual {v0, v1, v1}, Landroid/view/Window;->setFlags(II)V

    .line 242
    return-void
.end method

.method public static native get360NotifyUri()Ljava/lang/String;
.end method

.method public static native get360PrivateKey(Ljava/lang/String;)Ljava/lang/String;
.end method

.method public static native get360Token()Ljava/lang/String;
.end method

.method public static native get360UserId()Ljava/lang/String;
.end method

.method public static native get360selfPrivateKey()Ljava/lang/String;
.end method

.method public static native get91PrivateKey(Ljava/lang/String;)Ljava/lang/String;
.end method

.method public static getAndroidDeviceTypeString()Ljava/lang/String;
    .locals 2

    .prologue
    .line 108
    sget-object v0, Landroid/os/Build;->MODEL:Ljava/lang/String;

    .line 109
    .local v0, manufactureString:Ljava/lang/String;
    sget-object v1, Ljava/lang/System;->out:Ljava/io/PrintStream;

    invoke-virtual {v1, v0}, Ljava/io/PrintStream;->println(Ljava/lang/String;)V

    .line 110
    return-object v0
.end method

.method public static getAndroidSystemVersionString()Ljava/lang/String;
    .locals 9

    .prologue
    .line 119
    const/4 v4, 0x0

    .line 121
    .local v4, versionString:Ljava/lang/String;
    :try_start_0
    const-string v5, "android.os.SystemProperties"

    invoke-static {v5}, Ljava/lang/Class;->forName(Ljava/lang/String;)Ljava/lang/Class;

    move-result-object v2

    .line 122
    .local v2, mClassType:Ljava/lang/Class;,"Ljava/lang/Class<*>;"
    const-string v5, "get"

    const/4 v6, 0x1

    new-array v6, v6, [Ljava/lang/Class;

    const/4 v7, 0x0

    const-class v8, Ljava/lang/String;

    aput-object v8, v6, v7

    invoke-virtual {v2, v5, v6}, Ljava/lang/Class;->getDeclaredMethod(Ljava/lang/String;[Ljava/lang/Class;)Ljava/lang/reflect/Method;

    move-result-object v3

    .line 124
    .local v3, mGetMethod:Ljava/lang/reflect/Method;
    const/4 v5, 0x1

    new-array v5, v5, [Ljava/lang/Object;

    const/4 v6, 0x0

    const-string v7, "ro.build.id"

    aput-object v7, v5, v6

    invoke-virtual {v3, v2, v5}, Ljava/lang/reflect/Method;->invoke(Ljava/lang/Object;[Ljava/lang/Object;)Ljava/lang/Object;

    move-result-object v5

    move-object v0, v5

    check-cast v0, Ljava/lang/String;

    move-object v4, v0
    :try_end_0
    .catch Ljava/lang/Exception; {:try_start_0 .. :try_end_0} :catch_0

    .line 128
    .end local v2           #mClassType:Ljava/lang/Class;,"Ljava/lang/Class<*>;"
    .end local v3           #mGetMethod:Ljava/lang/reflect/Method;
    :goto_0
    sget-object v5, Ljava/lang/System;->out:Ljava/io/PrintStream;

    invoke-virtual {v5, v4}, Ljava/io/PrintStream;->println(Ljava/lang/String;)V

    .line 129
    return-object v4

    .line 125
    :catch_0
    move-exception v1

    .line 126
    .local v1, e:Ljava/lang/Exception;
    invoke-virtual {v1}, Ljava/lang/Exception;->printStackTrace()V

    goto :goto_0
.end method

.method public static native getAppID()Ljava/lang/String;
.end method

.method public static native getAppKey()Ljava/lang/String;
.end method

.method public static native getBaiduPrivateKey(Ljava/lang/String;)Ljava/lang/String;
.end method

.method public static getBusinessPlatform()I
    .locals 1

    .prologue
    .line 245
    const/4 v0, 0x0

    return v0
.end method

.method public static native getDownjoyPrivateKey(Ljava/lang/String;)Ljava/lang/String;
.end method

.method public static native getFixedProductId(Ljava/lang/String;)Ljava/lang/String;
.end method

.method public static native getLocojoyPayCheckUrl1()Ljava/lang/String;
.end method

.method public static native getLocojoyPayCheckUrl2()Ljava/lang/String;
.end method

.method public static native getLocojoyPayUrl(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;
.end method

.method public static getMacAddress()Ljava/lang/String;
    .locals 6

    .prologue
    .line 46
    const-string v0, "38AA3CD3D8E7"

    .line 47
    .local v0, buffer:Ljava/lang/String;
    sget-object v4, Lcom/locojoy/common/LocojoyCommonMethod;->wifi:Landroid/net/wifi/WifiManager;

    if-eqz v4, :cond_2

    sget-object v4, Lcom/locojoy/common/LocojoyCommonMethod;->s_activity:Landroid/app/Activity;

    if-eqz v4, :cond_2

    .line 49
    sget-object v4, Lcom/locojoy/common/LocojoyCommonMethod;->wifi:Landroid/net/wifi/WifiManager;

    invoke-virtual {v4}, Landroid/net/wifi/WifiManager;->isWifiEnabled()Z

    move-result v3

    .line 50
    .local v3, wifiFlag:Z
    sget-object v4, Lcom/locojoy/common/LocojoyCommonMethod;->wifi:Landroid/net/wifi/WifiManager;

    invoke-virtual {v4}, Landroid/net/wifi/WifiManager;->isWifiEnabled()Z

    move-result v4

    if-nez v4, :cond_0

    .line 52
    sget-object v4, Lcom/locojoy/common/LocojoyCommonMethod;->wifi:Landroid/net/wifi/WifiManager;

    const/4 v5, 0x1

    invoke-virtual {v4, v5}, Landroid/net/wifi/WifiManager;->setWifiEnabled(Z)Z

    .line 54
    :cond_0
    sget-object v4, Lcom/locojoy/common/LocojoyCommonMethod;->wifi:Landroid/net/wifi/WifiManager;

    invoke-virtual {v4}, Landroid/net/wifi/WifiManager;->getConnectionInfo()Landroid/net/wifi/WifiInfo;

    move-result-object v1

    .line 55
    .local v1, info:Landroid/net/wifi/WifiInfo;
    if-eqz v1, :cond_1

    .line 57
    invoke-virtual {v1}, Landroid/net/wifi/WifiInfo;->getMacAddress()Ljava/lang/String;

    move-result-object v2

    .line 58
    .local v2, macAddress:Ljava/lang/String;
    const-string v4, ":"

    const-string v5, ""

    invoke-virtual {v2, v4, v5}, Ljava/lang/String;->replaceAll(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;

    move-result-object v0

    .line 60
    .end local v2           #macAddress:Ljava/lang/String;
    :cond_1
    sget-object v4, Lcom/locojoy/common/LocojoyCommonMethod;->wifi:Landroid/net/wifi/WifiManager;

    invoke-virtual {v4, v3}, Landroid/net/wifi/WifiManager;->setWifiEnabled(Z)Z

    .line 62
    .end local v1           #info:Landroid/net/wifi/WifiInfo;
    .end local v3           #wifiFlag:Z
    :cond_2
    return-object v0
.end method

.method public static native getMerchantId()Ljava/lang/String;
.end method

.method public static native getMiPrivateKey(Ljava/lang/String;)Ljava/lang/String;
.end method

.method public static getPacageInfo()Ljava/lang/String;
    .locals 1

    .prologue
    .line 180
    sget-object v0, Lcom/locojoy/common/LocojoyCommonMethod;->s_activity:Landroid/app/Activity;

    if-eqz v0, :cond_0

    .line 182
    sget-object v0, Lcom/locojoy/common/LocojoyCommonMethod;->s_activity:Landroid/app/Activity;

    invoke-virtual {v0}, Landroid/app/Activity;->getPackageName()Ljava/lang/String;

    move-result-object v0

    .line 184
    :goto_0
    return-object v0

    :cond_0
    const/4 v0, 0x0

    goto :goto_0
.end method

.method public static native getPaymentKey()Ljava/lang/String;
.end method

.method public static native getPunchboxPrivateKey(Ljava/lang/String;)Ljava/lang/String;
.end method

.method public static native getServerSeqNum()Ljava/lang/String;
.end method

.method public static native getUcGameId()Ljava/lang/String;
.end method

.method public static native getUcPrivateKey(Ljava/lang/String;)Ljava/lang/String;
.end method

.method public static native getUcServerId()Ljava/lang/String;
.end method

.method public static native getUserId()Ljava/lang/String;
.end method

.method public static native getUserName()Ljava/lang/String;
.end method

.method public static getVersion()Ljava/lang/String;
    .locals 5

    .prologue
    .line 188
    invoke-static {}, Lcom/locojoy/common/LocojoyCommonMethod;->getPacageInfo()Ljava/lang/String;

    move-result-object v2

    .line 189
    .local v2, packageName:Ljava/lang/String;
    sget-object v3, Lcom/locojoy/common/LocojoyCommonMethod;->s_activity:Landroid/app/Activity;

    if-eqz v3, :cond_0

    sget-object v3, Lcom/locojoy/common/LocojoyCommonMethod;->packageManger:Landroid/content/pm/PackageManager;

    if-eqz v3, :cond_0

    const-string v3, ""

    invoke-virtual {v2, v3}, Ljava/lang/String;->equals(Ljava/lang/Object;)Z

    move-result v3

    if-nez v3, :cond_0

    .line 192
    :try_start_0
    sget-object v3, Lcom/locojoy/common/LocojoyCommonMethod;->packageManger:Landroid/content/pm/PackageManager;

    const/4 v4, 0x0

    invoke-virtual {v3, v2, v4}, Landroid/content/pm/PackageManager;->getPackageInfo(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;

    move-result-object v1

    .line 193
    .local v1, info:Landroid/content/pm/PackageInfo;
    iget-object v3, v1, Landroid/content/pm/PackageInfo;->versionName:Ljava/lang/String;
    :try_end_0
    .catch Landroid/content/pm/PackageManager$NameNotFoundException; {:try_start_0 .. :try_end_0} :catch_0

    .line 200
    .end local v1           #info:Landroid/content/pm/PackageInfo;
    :goto_0
    return-object v3

    .line 194
    :catch_0
    move-exception v0

    .line 197
    .local v0, e:Landroid/content/pm/PackageManager$NameNotFoundException;
    invoke-virtual {v0}, Landroid/content/pm/PackageManager$NameNotFoundException;->printStackTrace()V

    .line 200
    .end local v0           #e:Landroid/content/pm/PackageManager$NameNotFoundException;
    :cond_0
    const/4 v3, 0x0

    goto :goto_0
.end method

.method public static initManger()Z
    .locals 3

    .prologue
    .line 70
    const/4 v0, 0x0

    .line 71
    .local v0, flag:Z
    sget-object v1, Lcom/locojoy/common/LocojoyCommonMethod;->s_activity:Landroid/app/Activity;

    if-nez v1, :cond_0

    .line 73
    const/4 v0, 0x0

    .line 99
    :goto_0
    return v0

    .line 76
    :cond_0
    sget-object v1, Lcom/locojoy/common/LocojoyCommonMethod;->s_activity:Landroid/app/Activity;

    const-string v2, "wifi"

    invoke-virtual {v1, v2}, Landroid/app/Activity;->getSystemService(Ljava/lang/String;)Ljava/lang/Object;

    move-result-object v1

    check-cast v1, Landroid/net/wifi/WifiManager;

    sput-object v1, Lcom/locojoy/common/LocojoyCommonMethod;->wifi:Landroid/net/wifi/WifiManager;

    .line 78
    invoke-static {}, Lcom/locojoy/common/LocojoyCommonMethod;->isNetworkAvailable()Z

    .line 93
    sget-object v1, Lcom/locojoy/common/LocojoyCommonMethod;->s_activity:Landroid/app/Activity;

    invoke-virtual {v1}, Landroid/app/Activity;->getResources()Landroid/content/res/Resources;

    move-result-object v1

    invoke-virtual {v1}, Landroid/content/res/Resources;->getAssets()Landroid/content/res/AssetManager;

    move-result-object v1

    sput-object v1, Lcom/locojoy/common/LocojoyCommonMethod;->assets:Landroid/content/res/AssetManager;

    .line 95
    sget-object v1, Lcom/locojoy/common/LocojoyCommonMethod;->s_activity:Landroid/app/Activity;

    invoke-virtual {v1}, Landroid/app/Activity;->getPackageManager()Landroid/content/pm/PackageManager;

    move-result-object v1

    sput-object v1, Lcom/locojoy/common/LocojoyCommonMethod;->packageManger:Landroid/content/pm/PackageManager;

    .line 96
    const/4 v0, 0x1

    .line 99
    goto :goto_0
.end method

.method public static isNetworkAvailable()Z
    .locals 4

    .prologue
    .line 226
    sget-object v2, Lcom/locojoy/common/LocojoyCommonMethod;->s_activity:Landroid/app/Activity;

    .line 227
    const-string v3, "connectivity"

    invoke-virtual {v2, v3}, Landroid/app/Activity;->getSystemService(Ljava/lang/String;)Ljava/lang/Object;

    move-result-object v0

    .line 226
    check-cast v0, Landroid/net/ConnectivityManager;

    .line 228
    .local v0, cm:Landroid/net/ConnectivityManager;
    invoke-virtual {v0}, Landroid/net/ConnectivityManager;->getActiveNetworkInfo()Landroid/net/NetworkInfo;

    move-result-object v1

    .line 229
    .local v1, info:Landroid/net/NetworkInfo;
    if-eqz v1, :cond_0

    invoke-virtual {v1}, Landroid/net/NetworkInfo;->getState()Landroid/net/NetworkInfo$State;

    move-result-object v2

    sget-object v3, Landroid/net/NetworkInfo$State;->CONNECTED:Landroid/net/NetworkInfo$State;

    if-ne v2, v3, :cond_0

    .line 230
    const/4 v2, 0x1

    .line 231
    :goto_0
    return v2

    :cond_0
    const/4 v2, 0x0

    goto :goto_0
.end method

.method public static list(Ljava/lang/String;)[Ljava/lang/String;
    .locals 9
    .parameter "relative"

    .prologue
    const/4 v8, 0x3

    const/4 v7, 0x2

    const/4 v6, 0x1

    const/4 v5, 0x0

    .line 137
    :goto_0
    invoke-virtual {p0}, Ljava/lang/String;->length()I

    move-result v3

    if-le v3, v6, :cond_0

    invoke-virtual {p0, v5, v6}, Ljava/lang/String;->substring(II)Ljava/lang/String;

    move-result-object v3

    const-string v4, "/"

    invoke-virtual {v3, v4}, Ljava/lang/String;->equals(Ljava/lang/Object;)Z

    move-result v3

    if-nez v3, :cond_4

    .line 142
    :cond_0
    :goto_1
    invoke-virtual {p0}, Ljava/lang/String;->length()I

    move-result v3

    if-le v3, v7, :cond_1

    invoke-virtual {p0, v5, v7}, Ljava/lang/String;->substring(II)Ljava/lang/String;

    move-result-object v3

    const-string v4, "./"

    invoke-virtual {v3, v4}, Ljava/lang/String;->equals(Ljava/lang/Object;)Z

    move-result v3

    if-nez v3, :cond_5

    .line 147
    :cond_1
    :goto_2
    invoke-virtual {p0}, Ljava/lang/String;->length()I

    move-result v3

    if-le v3, v8, :cond_2

    invoke-virtual {p0, v5, v8}, Ljava/lang/String;->substring(II)Ljava/lang/String;

    move-result-object v3

    const-string v4, "../"

    invoke-virtual {v3, v4}, Ljava/lang/String;->equals(Ljava/lang/Object;)Z

    move-result v3

    if-nez v3, :cond_6

    .line 152
    :cond_2
    :goto_3
    invoke-virtual {p0}, Ljava/lang/String;->length()I

    move-result v3

    if-le v3, v6, :cond_3

    .line 153
    invoke-virtual {p0}, Ljava/lang/String;->length()I

    move-result v3

    add-int/lit8 v3, v3, -0x1

    invoke-virtual {p0, v3}, Ljava/lang/String;->substring(I)Ljava/lang/String;

    move-result-object v3

    const-string v4, "/"

    invoke-virtual {v3, v4}, Ljava/lang/String;->equals(Ljava/lang/Object;)Z

    move-result v3

    if-nez v3, :cond_7

    .line 159
    :cond_3
    :try_start_0
    sget-object v3, Lcom/locojoy/common/LocojoyCommonMethod;->assets:Landroid/content/res/AssetManager;

    invoke-virtual {v3, p0}, Landroid/content/res/AssetManager;->list(Ljava/lang/String;)[Ljava/lang/String;

    move-result-object v2

    .line 161
    .local v2, ret:[Ljava/lang/String;
    const/4 v1, 0x0

    .local v1, i:I
    :goto_4
    array-length v3, v2
    :try_end_0
    .catch Ljava/io/IOException; {:try_start_0 .. :try_end_0} :catch_0

    if-lt v1, v3, :cond_8

    .line 170
    .end local v1           #i:I
    .end local v2           #ret:[Ljava/lang/String;
    :goto_5
    return-object v2

    .line 138
    :cond_4
    invoke-virtual {p0, v6}, Ljava/lang/String;->substring(I)Ljava/lang/String;

    move-result-object p0

    goto :goto_0

    .line 143
    :cond_5
    invoke-virtual {p0, v7}, Ljava/lang/String;->substring(I)Ljava/lang/String;

    move-result-object p0

    goto :goto_1

    .line 148
    :cond_6
    invoke-virtual {p0, v8}, Ljava/lang/String;->substring(I)Ljava/lang/String;

    move-result-object p0

    goto :goto_2

    .line 154
    :cond_7
    invoke-virtual {p0}, Ljava/lang/String;->length()I

    move-result v3

    add-int/lit8 v3, v3, -0x1

    invoke-virtual {p0, v5, v3}, Ljava/lang/String;->substring(II)Ljava/lang/String;

    move-result-object p0

    goto :goto_3

    .line 161
    .restart local v1       #i:I
    .restart local v2       #ret:[Ljava/lang/String;
    :cond_8
    add-int/lit8 v1, v1, 0x1

    goto :goto_4

    .line 167
    .end local v1           #i:I
    .end local v2           #ret:[Ljava/lang/String;
    :catch_0
    move-exception v0

    .line 169
    .local v0, ex:Ljava/io/IOException;
    const-string v3, "cocos2d-x debug info"

    new-instance v4, Ljava/lang/StringBuilder;

    const-string v5, "failed to list resources in "

    invoke-direct {v4, v5}, Ljava/lang/StringBuilder;-><init>(Ljava/lang/String;)V

    invoke-virtual {v4, p0}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    move-result-object v4

    invoke-virtual {v4}, Ljava/lang/StringBuilder;->toString()Ljava/lang/String;

    move-result-object v4

    invoke-static {v3, v4}, Landroid/util/Log;->d(Ljava/lang/String;Ljava/lang/String;)I

    .line 170
    sget-object v2, Lcom/locojoy/common/LocojoyCommonMethod;->empty:[Ljava/lang/String;

    goto :goto_5
.end method

.method public static openURL(Ljava/lang/String;)V
    .locals 2
    .parameter "url"

    .prologue
    .line 209
    new-instance v0, Landroid/content/Intent;

    invoke-direct {v0}, Landroid/content/Intent;-><init>()V

    .line 210
    .local v0, intent:Landroid/content/Intent;
    invoke-static {p0}, Landroid/net/Uri;->parse(Ljava/lang/String;)Landroid/net/Uri;

    move-result-object v1

    invoke-virtual {v0, v1}, Landroid/content/Intent;->setData(Landroid/net/Uri;)Landroid/content/Intent;

    .line 211
    const-string v1, "android.intent.action.VIEW"

    invoke-virtual {v0, v1}, Landroid/content/Intent;->setAction(Ljava/lang/String;)Landroid/content/Intent;

    .line 212
    sget-object v1, Lcom/locojoy/common/LocojoyCommonMethod;->s_activity:Landroid/app/Activity;

    if-eqz v1, :cond_0

    .line 214
    sget-object v1, Lcom/locojoy/common/LocojoyCommonMethod;->s_activity:Landroid/app/Activity;

    invoke-virtual {v1, v0}, Landroid/app/Activity;->startActivity(Landroid/content/Intent;)V

    .line 216
    :cond_0
    return-void
.end method

.method public static setActivity(Landroid/app/Activity;)V
    .locals 0
    .parameter "_activity"

    .prologue
    .line 35
    sput-object p0, Lcom/locojoy/common/LocojoyCommonMethod;->s_activity:Landroid/app/Activity;

    .line 36
    invoke-static {}, Lcom/locojoy/common/LocojoyCommonMethod;->initManger()Z

    .line 37
    return-void
.end method
