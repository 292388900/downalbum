.class public Lcom/locojoy/immt_a_chs/IamMT;
.super Lorg/cocos2dx/lib/Cocos2dxActivity;
.source "IamMT.java"

# interfaces
.implements Lcom/locojoy/immt_a_chs/Info;


# static fields
.field private static handler:Lcom/locojoy/PurchaseHandler;

.field private static packageName:Ljava/lang/String;


# instance fields
.field private framelayout:Landroid/widget/FrameLayout;

.field private logoView:Landroid/view/View;

.field private mGLView:Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;


# direct methods
.method static constructor <clinit>()V
    .locals 1

    .prologue
    .line 61
    const/4 v0, 0x0

    sput-object v0, Lcom/locojoy/immt_a_chs/IamMT;->packageName:Ljava/lang/String;

    .line 240
    const-string v0, "game"

    invoke-static {v0}, Ljava/lang/System;->loadLibrary(Ljava/lang/String;)V

    .line 241
    return-void
.end method

.method public constructor <init>()V
    .locals 0

    .prologue
    .line 53
    invoke-direct {p0}, Lorg/cocos2dx/lib/Cocos2dxActivity;-><init>()V

    return-void
.end method

.method public static chargeDiamondsOn(ILjava/lang/String;Ljava/lang/String;F)Ljava/lang/String;
    .locals 5
    .parameter "nProductNum"
    .parameter "ProductID"
    .parameter "szProductName"
    .parameter "fPrice"

    .prologue
    .line 173
    const-string v3, "IamMT"

    invoke-static {v3, p1}, Landroid/util/Log;->i(Ljava/lang/String;Ljava/lang/String;)I

    .line 175
    invoke-static {}, Ljava/util/UUID;->randomUUID()Ljava/util/UUID;

    move-result-object v3

    invoke-virtual {v3}, Ljava/util/UUID;->toString()Ljava/lang/String;

    move-result-object v2

    .line 176
    .local v2, order:Ljava/lang/String;
    const-string v3, "-"

    const-string v4, ""

    invoke-virtual {v4}, Ljava/lang/String;->trim()Ljava/lang/String;

    move-result-object v4

    invoke-virtual {v2, v3, v4}, Ljava/lang/String;->replace(Ljava/lang/CharSequence;Ljava/lang/CharSequence;)Ljava/lang/String;

    move-result-object v2

    .line 177
    new-instance v1, Landroid/os/Message;

    invoke-direct {v1}, Landroid/os/Message;-><init>()V

    .line 178
    .local v1, msg:Landroid/os/Message;
    new-instance v0, Landroid/os/Bundle;

    invoke-direct {v0}, Landroid/os/Bundle;-><init>()V

    .line 179
    .local v0, bundle:Landroid/os/Bundle;
    const-string v3, "fPrice"

    invoke-virtual {v0, v3, p3}, Landroid/os/Bundle;->putFloat(Ljava/lang/String;F)V

    .line 180
    const-string v3, "szProductName"

    invoke-virtual {v0, v3, p2}, Landroid/os/Bundle;->putString(Ljava/lang/String;Ljava/lang/String;)V

    .line 181
    const-string v3, "ProductID"

    invoke-virtual {v0, v3, p1}, Landroid/os/Bundle;->putString(Ljava/lang/String;Ljava/lang/String;)V

    .line 182
    const-string v3, "productNum"

    invoke-virtual {v0, v3, p0}, Landroid/os/Bundle;->putInt(Ljava/lang/String;I)V

    .line 183
    const-string v3, "order"

    invoke-virtual {v0, v3, v2}, Landroid/os/Bundle;->putString(Ljava/lang/String;Ljava/lang/String;)V

    .line 184
    invoke-virtual {v1, v0}, Landroid/os/Message;->setData(Landroid/os/Bundle;)V

    .line 185
    const/16 v3, 0x65

    iput v3, v1, Landroid/os/Message;->what:I

    .line 186
    sget-object v3, Lcom/locojoy/immt_a_chs/IamMT;->handler:Lcom/locojoy/PurchaseHandler;

    invoke-virtual {v3, v1}, Lcom/locojoy/PurchaseHandler;->sendMessage(Landroid/os/Message;)Z

    .line 187
    return-object v2
.end method

.method public static checkLeakOrder(Ljava/lang/String;)V
    .locals 5
    .parameter "order"

    .prologue
    .line 146
    const-string v2, "IamMT"

    new-instance v3, Ljava/lang/StringBuilder;

    const-string v4, "checkLeakOrder"

    invoke-direct {v3, v4}, Ljava/lang/StringBuilder;-><init>(Ljava/lang/String;)V

    invoke-virtual {v3, p0}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    move-result-object v3

    invoke-virtual {v3}, Ljava/lang/StringBuilder;->toString()Ljava/lang/String;

    move-result-object v3

    invoke-static {v2, v3}, Landroid/util/Log;->d(Ljava/lang/String;Ljava/lang/String;)I

    .line 147
    new-instance v1, Landroid/os/Message;

    invoke-direct {v1}, Landroid/os/Message;-><init>()V

    .line 148
    .local v1, msg:Landroid/os/Message;
    new-instance v0, Landroid/os/Bundle;

    invoke-direct {v0}, Landroid/os/Bundle;-><init>()V

    .line 149
    .local v0, bundle:Landroid/os/Bundle;
    const-string v2, "order"

    invoke-virtual {v0, v2, p0}, Landroid/os/Bundle;->putString(Ljava/lang/String;Ljava/lang/String;)V

    .line 150
    invoke-virtual {v1, v0}, Landroid/os/Message;->setData(Landroid/os/Bundle;)V

    .line 151
    const/16 v2, 0x68

    iput v2, v1, Landroid/os/Message;->what:I

    .line 152
    sget-object v2, Lcom/locojoy/immt_a_chs/IamMT;->handler:Lcom/locojoy/PurchaseHandler;

    invoke-virtual {v2, v1}, Lcom/locojoy/PurchaseHandler;->sendMessage(Landroid/os/Message;)Z

    .line 153
    return-void
.end method

.method public static checkVersion()V
    .locals 3

    .prologue
    .line 158
    new-instance v0, Landroid/os/Message;

    invoke-direct {v0}, Landroid/os/Message;-><init>()V

    .line 159
    .local v0, msg:Landroid/os/Message;
    const/16 v1, 0x69

    iput v1, v0, Landroid/os/Message;->what:I

    .line 160
    sget-object v1, Lcom/locojoy/immt_a_chs/IamMT;->handler:Lcom/locojoy/PurchaseHandler;

    invoke-virtual {v1, v0}, Lcom/locojoy/PurchaseHandler;->sendMessage(Landroid/os/Message;)Z

    .line 162
    const-string v1, "IamMT"

    const-string v2, "checkVersion"

    invoke-static {v1, v2}, Landroid/util/Log;->i(Ljava/lang/String;Ljava/lang/String;)I

    .line 163
    return-void
.end method

.method private detectOpenGLES20()Z
    .locals 4

    .prologue
    .line 209
    const-string v2, "activity"

    invoke-virtual {p0, v2}, Lcom/locojoy/immt_a_chs/IamMT;->getSystemService(Ljava/lang/String;)Ljava/lang/Object;

    move-result-object v0

    check-cast v0, Landroid/app/ActivityManager;

    .line 210
    .local v0, am:Landroid/app/ActivityManager;
    invoke-virtual {v0}, Landroid/app/ActivityManager;->getDeviceConfigurationInfo()Landroid/content/pm/ConfigurationInfo;

    move-result-object v1

    .line 212
    .local v1, info:Landroid/content/pm/ConfigurationInfo;
    iget v2, v1, Landroid/content/pm/ConfigurationInfo;->reqGlEsVersion:I

    const/high16 v3, 0x2

    if-ge v2, v3, :cond_0

    sget-object v2, Landroid/os/Build;->FINGERPRINT:Ljava/lang/String;

    .line 213
    const-string v3, "generic"

    invoke-virtual {v2, v3}, Ljava/lang/String;->startsWith(Ljava/lang/String;)Z

    move-result v2

    if-nez v2, :cond_0

    .line 212
    const/4 v2, 0x0

    :goto_0
    return v2

    :cond_0
    const/4 v2, 0x1

    goto :goto_0
.end method

.method public static enterPlatform()V
    .locals 2

    .prologue
    .line 139
    new-instance v0, Landroid/os/Message;

    invoke-direct {v0}, Landroid/os/Message;-><init>()V

    .line 140
    .local v0, msg:Landroid/os/Message;
    const/16 v1, 0x66

    iput v1, v0, Landroid/os/Message;->what:I

    .line 141
    sget-object v1, Lcom/locojoy/immt_a_chs/IamMT;->handler:Lcom/locojoy/PurchaseHandler;

    invoke-virtual {v1, v0}, Lcom/locojoy/PurchaseHandler;->sendMessage(Landroid/os/Message;)Z

    .line 143
    return-void
.end method

.method public static login()V
    .locals 2

    .prologue
    .line 129
    new-instance v0, Landroid/os/Message;

    invoke-direct {v0}, Landroid/os/Message;-><init>()V

    .line 130
    .local v0, msg:Landroid/os/Message;
    const/16 v1, 0x64

    iput v1, v0, Landroid/os/Message;->what:I

    .line 131
    sget-object v1, Lcom/locojoy/immt_a_chs/IamMT;->handler:Lcom/locojoy/PurchaseHandler;

    invoke-virtual {v1, v0}, Lcom/locojoy/PurchaseHandler;->sendMessage(Landroid/os/Message;)Z

    .line 132
    return-void
.end method


# virtual methods
.method public checkDevice()V
    .locals 3

    .prologue
    .line 220
    sget-object v0, Landroid/os/Build;->MODEL:Ljava/lang/String;

    const-string v1, "sdk"

    invoke-virtual {v0, v1}, Ljava/lang/String;->equals(Ljava/lang/Object;)Z

    move-result v0

    if-nez v0, :cond_0

    sget-object v0, Landroid/os/Build;->MODEL:Ljava/lang/String;

    const-string v1, "google_sdk"

    invoke-virtual {v0, v1}, Ljava/lang/String;->equals(Ljava/lang/Object;)Z

    move-result v0

    if-eqz v0, :cond_1

    .line 221
    :cond_0
    new-instance v0, Landroid/app/AlertDialog$Builder;

    invoke-direct {v0, p0}, Landroid/app/AlertDialog$Builder;-><init>(Landroid/content/Context;)V

    .line 223
    invoke-virtual {p0}, Lcom/locojoy/immt_a_chs/IamMT;->getResources()Landroid/content/res/Resources;

    move-result-object v1

    .line 224
    const v2, 0x7f040007

    invoke-virtual {v1, v2}, Landroid/content/res/Resources;->getString(I)Ljava/lang/String;

    move-result-object v1

    .line 222
    invoke-virtual {v0, v1}, Landroid/app/AlertDialog$Builder;->setMessage(Ljava/lang/CharSequence;)Landroid/app/AlertDialog$Builder;

    move-result-object v0

    .line 226
    invoke-virtual {p0}, Lcom/locojoy/immt_a_chs/IamMT;->getResources()Landroid/content/res/Resources;

    move-result-object v1

    const v2, 0x7f040008

    invoke-virtual {v1, v2}, Landroid/content/res/Resources;->getString(I)Ljava/lang/String;

    move-result-object v1

    .line 227
    new-instance v2, Lcom/locojoy/immt_a_chs/IamMT$1;

    invoke-direct {v2, p0}, Lcom/locojoy/immt_a_chs/IamMT$1;-><init>(Lcom/locojoy/immt_a_chs/IamMT;)V

    .line 225
    invoke-virtual {v0, v1, v2}, Landroid/app/AlertDialog$Builder;->setPositiveButton(Ljava/lang/CharSequence;Landroid/content/DialogInterface$OnClickListener;)Landroid/app/AlertDialog$Builder;

    move-result-object v0

    .line 235
    invoke-virtual {v0}, Landroid/app/AlertDialog$Builder;->show()Landroid/app/AlertDialog;

    move-result-object v0

    const/4 v1, 0x0

    invoke-virtual {v0, v1}, Landroid/app/AlertDialog;->setCancelable(Z)V

    .line 237
    :cond_1
    return-void
.end method

.method public dismissLogo()V
    .locals 0

    .prologue
    .line 126
    return-void
.end method

.method protected onCreate(Landroid/os/Bundle;)V
    .locals 6
    .parameter "savedInstanceState"

    .prologue
    const/4 v5, -0x1

    .line 65
    invoke-super {p0, p1}, Lorg/cocos2dx/lib/Cocos2dxActivity;->onCreate(Landroid/os/Bundle;)V

    .line 66
    invoke-virtual {p0}, Lcom/locojoy/immt_a_chs/IamMT;->checkDevice()V

    .line 67
    invoke-direct {p0}, Lcom/locojoy/immt_a_chs/IamMT;->detectOpenGLES20()Z

    move-result v4

    if-eqz v4, :cond_0

    .line 70
    new-instance v4, Lcom/locojoy/PurchaseHandler;

    invoke-direct {v4, p0}, Lcom/locojoy/PurchaseHandler;-><init>(Landroid/content/Context;)V

    sput-object v4, Lcom/locojoy/immt_a_chs/IamMT;->handler:Lcom/locojoy/PurchaseHandler;

    .line 73
    invoke-virtual {p0}, Lcom/locojoy/immt_a_chs/IamMT;->getApplication()Landroid/app/Application;

    move-result-object v4

    invoke-virtual {v4}, Landroid/app/Application;->getPackageName()Ljava/lang/String;

    move-result-object v4

    sput-object v4, Lcom/locojoy/immt_a_chs/IamMT;->packageName:Ljava/lang/String;

    .line 74
    sget-object v4, Lcom/locojoy/immt_a_chs/IamMT;->packageName:Ljava/lang/String;

    invoke-super {p0, v4}, Lorg/cocos2dx/lib/Cocos2dxActivity;->setPackageName(Ljava/lang/String;)V

    .line 75
    invoke-static {p0}, Lcom/locojoy/common/LocojoyCommonMethod;->setActivity(Landroid/app/Activity;)V

    .line 77
    new-instance v3, Landroid/view/ViewGroup$LayoutParams;

    invoke-direct {v3, v5, v5}, Landroid/view/ViewGroup$LayoutParams;-><init>(II)V

    .line 81
    .local v3, framelayout_params:Landroid/view/ViewGroup$LayoutParams;
    new-instance v2, Landroid/widget/FrameLayout;

    invoke-direct {v2, p0}, Landroid/widget/FrameLayout;-><init>(Landroid/content/Context;)V

    .line 82
    .local v2, framelayout:Landroid/widget/FrameLayout;
    invoke-virtual {v2, v3}, Landroid/widget/FrameLayout;->setLayoutParams(Landroid/view/ViewGroup$LayoutParams;)V

    .line 85
    new-instance v1, Landroid/view/ViewGroup$LayoutParams;

    .line 87
    const/4 v4, -0x2

    .line 85
    invoke-direct {v1, v5, v4}, Landroid/view/ViewGroup$LayoutParams;-><init>(II)V

    .line 88
    .local v1, edittext_layout_params:Landroid/view/ViewGroup$LayoutParams;
    new-instance v0, Lorg/cocos2dx/lib/Cocos2dxEditText;

    invoke-direct {v0, p0}, Lorg/cocos2dx/lib/Cocos2dxEditText;-><init>(Landroid/content/Context;)V

    .line 89
    .local v0, edittext:Lorg/cocos2dx/lib/Cocos2dxEditText;
    invoke-virtual {v0, v1}, Lorg/cocos2dx/lib/Cocos2dxEditText;->setLayoutParams(Landroid/view/ViewGroup$LayoutParams;)V

    .line 92
    invoke-virtual {v2, v0}, Landroid/widget/FrameLayout;->addView(Landroid/view/View;)V

    .line 95
    new-instance v4, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;

    invoke-direct {v4, p0}, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;-><init>(Landroid/content/Context;)V

    iput-object v4, p0, Lcom/locojoy/immt_a_chs/IamMT;->mGLView:Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;

    .line 98
    iget-object v4, p0, Lcom/locojoy/immt_a_chs/IamMT;->mGLView:Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;

    invoke-virtual {v2, v4}, Landroid/widget/FrameLayout;->addView(Landroid/view/View;)V

    .line 100
    iget-object v4, p0, Lcom/locojoy/immt_a_chs/IamMT;->mGLView:Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;

    const/4 v5, 0x2

    invoke-virtual {v4, v5}, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;->setEGLContextClientVersion(I)V

    .line 101
    iget-object v4, p0, Lcom/locojoy/immt_a_chs/IamMT;->mGLView:Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;

    new-instance v5, Lorg/cocos2dx/lib/Cocos2dxRenderer;

    invoke-direct {v5}, Lorg/cocos2dx/lib/Cocos2dxRenderer;-><init>()V

    invoke-virtual {v4, v5}, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;->setCocos2dxRenderer(Lorg/cocos2dx/lib/Cocos2dxRenderer;)V

    .line 102
    iget-object v4, p0, Lcom/locojoy/immt_a_chs/IamMT;->mGLView:Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;

    invoke-virtual {v4, v0}, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;->setTextField(Lorg/cocos2dx/lib/Cocos2dxEditText;)V

    .line 105
    invoke-virtual {p0, v2}, Lcom/locojoy/immt_a_chs/IamMT;->setContentView(Landroid/view/View;)V

    .line 110
    .end local v0           #edittext:Lorg/cocos2dx/lib/Cocos2dxEditText;
    .end local v1           #edittext_layout_params:Landroid/view/ViewGroup$LayoutParams;
    .end local v2           #framelayout:Landroid/widget/FrameLayout;
    .end local v3           #framelayout_params:Landroid/view/ViewGroup$LayoutParams;
    :goto_0
    invoke-static {}, Lcom/locojoy/immt_a_chs/IamMT;->login()V

    .line 111
    return-void

    .line 107
    :cond_0
    const-string v4, "activity"

    const-string v5, "don\'t support gles2.0"

    invoke-static {v4, v5}, Landroid/util/Log;->d(Ljava/lang/String;Ljava/lang/String;)I

    .line 108
    invoke-virtual {p0}, Lcom/locojoy/immt_a_chs/IamMT;->finish()V

    goto :goto_0
.end method

.method public onKeyDown(ILandroid/view/KeyEvent;)Z
    .locals 1
    .parameter "keyCode"
    .parameter "event"

    .prologue
    .line 250
    invoke-super {p0, p1, p2}, Lorg/cocos2dx/lib/Cocos2dxActivity;->onKeyDown(ILandroid/view/KeyEvent;)Z

    move-result v0

    return v0
.end method

.method protected onPause()V
    .locals 1

    .prologue
    .line 191
    invoke-super {p0}, Lorg/cocos2dx/lib/Cocos2dxActivity;->onPause()V

    .line 192
    iget-object v0, p0, Lcom/locojoy/immt_a_chs/IamMT;->mGLView:Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;

    invoke-virtual {v0}, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;->onPause()V

    .line 193
    return-void
.end method

.method protected onResume()V
    .locals 1

    .prologue
    .line 204
    invoke-super {p0}, Lorg/cocos2dx/lib/Cocos2dxActivity;->onResume()V

    .line 205
    iget-object v0, p0, Lcom/locojoy/immt_a_chs/IamMT;->mGLView:Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;

    invoke-virtual {v0}, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;->onResume()V

    .line 206
    return-void
.end method

.method public showLogo()V
    .locals 0

    .prologue
    .line 121
    return-void
.end method
