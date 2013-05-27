.class public Lcom/locojoy/PurchaseHandler;
.super Landroid/os/Handler;
.source "PurchaseHandler.java"

# interfaces
.implements Lcom/locojoy/immt_a_chs/Info;


# static fields
.field private static bIsVersionMatched:Z

.field private static context:Landroid/content/Context;

.field private static pathNo:Ljava/lang/String;

.field private static purchaseHandler:Lcom/locojoy/PurchaseHandler;


# instance fields
.field private isHideLoading:Z

.field private isValidFlag:Z

.field private progressDialog:Landroid/app/ProgressDialog;

.field private purchView:Landroid/webkit/WebView;

.field private purchaseDialog:Landroid/app/AlertDialog;


# direct methods
.method static constructor <clinit>()V
    .locals 1

    .prologue
    .line 33
    const/4 v0, 0x1

    sput-boolean v0, Lcom/locojoy/PurchaseHandler;->bIsVersionMatched:Z

    .line 34
    const-string v0, "0100"

    sput-object v0, Lcom/locojoy/PurchaseHandler;->pathNo:Ljava/lang/String;

    return-void
.end method

.method public constructor <init>(Landroid/content/Context;)V
    .locals 2
    .parameter "_context"

    .prologue
    const/4 v0, 0x0

    .line 38
    invoke-direct {p0}, Landroid/os/Handler;-><init>()V

    .line 31
    iput-boolean v0, p0, Lcom/locojoy/PurchaseHandler;->isValidFlag:Z

    .line 32
    iput-boolean v0, p0, Lcom/locojoy/PurchaseHandler;->isHideLoading:Z

    .line 39
    sput-object p1, Lcom/locojoy/PurchaseHandler;->context:Landroid/content/Context;

    .line 41
    new-instance v0, Landroid/os/Handler;

    invoke-direct {v0}, Landroid/os/Handler;-><init>()V

    new-instance v1, Lcom/locojoy/PurchaseHandler$1;

    invoke-direct {v1, p0}, Lcom/locojoy/PurchaseHandler$1;-><init>(Lcom/locojoy/PurchaseHandler;)V

    invoke-virtual {v0, v1}, Landroid/os/Handler;->post(Ljava/lang/Runnable;)Z

    .line 49
    sput-object p0, Lcom/locojoy/PurchaseHandler;->purchaseHandler:Lcom/locojoy/PurchaseHandler;

    .line 50
    return-void
.end method

.method public static native LoginCallBack(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)I
.end method

.method public static native UniPayAsynResult(Ljava/lang/String;Ljava/lang/String;I)V
.end method

.method static synthetic access$0(Lcom/locojoy/PurchaseHandler;)V
    .locals 0
    .parameter

    .prologue
    .line 128
    invoke-direct {p0}, Lcom/locojoy/PurchaseHandler;->initSDK()V

    return-void
.end method

.method static synthetic access$1(Lcom/locojoy/PurchaseHandler;)V
    .locals 0
    .parameter

    .prologue
    .line 174
    invoke-direct {p0}, Lcom/locojoy/PurchaseHandler;->accountLogin()V

    return-void
.end method

.method private accountLogin()V
    .locals 4

    .prologue
    .line 176
    const-string v0, ""

    .line 177
    .local v0, Uin:Ljava/lang/String;
    const-string v1, ""

    .line 178
    .local v1, niceName:Ljava/lang/String;
    const-string v2, ""

    .line 179
    .local v2, token:Ljava/lang/String;
    sget-object v3, Lcom/locojoy/PurchaseHandler;->pathNo:Ljava/lang/String;

    invoke-static {v0, v1, v2, v3}, Lcom/locojoy/PurchaseHandler;->LoginCallBack(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)I

    .line 180
    return-void
.end method

.method public static native checkPaySuccessDidFinish(Ljava/lang/String;I)V
.end method

.method private initApp()V
    .locals 0

    .prologue
    .line 168
    return-void
.end method

.method private initSDK()V
    .locals 0

    .prologue
    .line 131
    invoke-direct {p0}, Lcom/locojoy/PurchaseHandler;->initApp()V

    .line 133
    return-void
.end method

.method public static isVersionMatched()I
    .locals 1

    .prologue
    .line 262
    sget-boolean v0, Lcom/locojoy/PurchaseHandler;->bIsVersionMatched:Z

    if-eqz v0, :cond_0

    .line 263
    const/4 v0, 0x1

    .line 266
    :goto_0
    return v0

    :cond_0
    const/4 v0, 0x0

    goto :goto_0
.end method


# virtual methods
.method public appVersionUpdateDidFinish()V
    .locals 0

    .prologue
    .line 259
    return-void
.end method

.method public checkOrder(Ljava/lang/String;)V
    .locals 0
    .parameter "order"

    .prologue
    .line 123
    return-void
.end method

.method public enter91BBS()V
    .locals 0

    .prologue
    .line 187
    return-void
.end method

.method public handleMessage(Landroid/os/Message;)V
    .locals 8
    .parameter "msg"

    .prologue
    .line 55
    invoke-super {p0, p1}, Landroid/os/Handler;->handleMessage(Landroid/os/Message;)V

    .line 56
    invoke-virtual {p1}, Landroid/os/Message;->getData()Landroid/os/Bundle;

    move-result-object v6

    .line 57
    .local v6, bundle:Landroid/os/Bundle;
    iget v0, p1, Landroid/os/Message;->what:I

    packed-switch v0, :pswitch_data_0

    .line 99
    :goto_0
    :pswitch_0
    return-void

    .line 60
    :pswitch_1
    new-instance v0, Lcom/locojoy/PurchaseHandler$2;

    invoke-direct {v0, p0}, Lcom/locojoy/PurchaseHandler$2;-><init>(Lcom/locojoy/PurchaseHandler;)V

    invoke-virtual {p0, v0}, Lcom/locojoy/PurchaseHandler;->post(Ljava/lang/Runnable;)Z

    goto :goto_0

    .line 75
    :pswitch_2
    const-string v0, "fPrice"

    invoke-virtual {v6, v0}, Landroid/os/Bundle;->getFloat(Ljava/lang/String;)F

    move-result v2

    .line 76
    .local v2, fPrice:F
    const-string v0, "ProductID"

    invoke-virtual {v6, v0}, Landroid/os/Bundle;->getString(Ljava/lang/String;)Ljava/lang/String;

    move-result-object v3

    .line 77
    .local v3, productId:Ljava/lang/String;
    const-string v0, "szProductName"

    invoke-virtual {v6, v0}, Landroid/os/Bundle;->getString(Ljava/lang/String;)Ljava/lang/String;

    move-result-object v1

    .line 78
    .local v1, szProductName:Ljava/lang/String;
    const-string v0, "order"

    invoke-virtual {v6, v0}, Landroid/os/Bundle;->getString(Ljava/lang/String;)Ljava/lang/String;

    move-result-object v4

    .line 79
    .local v4, order:Ljava/lang/String;
    const-string v0, "productNum"

    invoke-virtual {v6, v0}, Landroid/os/Bundle;->getInt(Ljava/lang/String;)I

    move-result v5

    .local v5, productNum:I
    move-object v0, p0

    .line 80
    invoke-virtual/range {v0 .. v5}, Lcom/locojoy/PurchaseHandler;->requespurchase(Ljava/lang/String;FLjava/lang/String;Ljava/lang/String;I)V

    goto :goto_0

    .line 89
    .end local v1           #szProductName:Ljava/lang/String;
    .end local v2           #fPrice:F
    .end local v3           #productId:Ljava/lang/String;
    .end local v4           #order:Ljava/lang/String;
    .end local v5           #productNum:I
    :pswitch_3
    const-string v0, "order"

    invoke-virtual {v6, v0}, Landroid/os/Bundle;->getString(Ljava/lang/String;)Ljava/lang/String;

    move-result-object v7

    .line 90
    .local v7, orderSeri:Ljava/lang/String;
    invoke-virtual {p0, v7}, Lcom/locojoy/PurchaseHandler;->checkOrder(Ljava/lang/String;)V

    goto :goto_0

    .line 93
    .end local v7           #orderSeri:Ljava/lang/String;
    :pswitch_4
    invoke-virtual {p0}, Lcom/locojoy/PurchaseHandler;->updateVersion()V

    goto :goto_0

    .line 57
    nop

    :pswitch_data_0
    .packed-switch 0x64
        :pswitch_1
        :pswitch_2
        :pswitch_0
        :pswitch_0
        :pswitch_3
        :pswitch_4
    .end packed-switch
.end method

.method protected hideLoading()V
    .locals 1

    .prologue
    .line 207
    iget-object v0, p0, Lcom/locojoy/PurchaseHandler;->progressDialog:Landroid/app/ProgressDialog;

    if-eqz v0, :cond_0

    .line 208
    iget-object v0, p0, Lcom/locojoy/PurchaseHandler;->progressDialog:Landroid/app/ProgressDialog;

    invoke-virtual {v0}, Landroid/app/ProgressDialog;->cancel()V

    .line 209
    const/4 v0, 0x0

    iput-object v0, p0, Lcom/locojoy/PurchaseHandler;->progressDialog:Landroid/app/ProgressDialog;

    .line 211
    :cond_0
    return-void
.end method

.method public requespurchase(Ljava/lang/String;FLjava/lang/String;Ljava/lang/String;I)V
    .locals 4
    .parameter "productName"
    .parameter "price"
    .parameter "product_id"
    .parameter "order"
    .parameter "productNum"
    .annotation build Landroid/annotation/SuppressLint;
        value = {
            "SetJavaScriptEnabled"
        }
    .end annotation

    .prologue
    .line 275
    new-instance v1, Landroid/content/Intent;

    invoke-direct {v1}, Landroid/content/Intent;-><init>()V

    .line 276
    .local v1, intent:Landroid/content/Intent;
    new-instance v0, Landroid/os/Bundle;

    invoke-direct {v0}, Landroid/os/Bundle;-><init>()V

    .line 277
    .local v0, bundle:Landroid/os/Bundle;
    const-string v2, "product_id"

    invoke-virtual {v0, v2, p3}, Landroid/os/Bundle;->putString(Ljava/lang/String;Ljava/lang/String;)V

    .line 278
    const-string v2, "order"

    invoke-virtual {v0, v2, p4}, Landroid/os/Bundle;->putString(Ljava/lang/String;Ljava/lang/String;)V

    .line 279
    invoke-virtual {v1, v0}, Landroid/content/Intent;->putExtras(Landroid/os/Bundle;)Landroid/content/Intent;

    .line 280
    sget-object v2, Lcom/locojoy/PurchaseHandler;->context:Landroid/content/Context;

    const-class v3, Lcom/locojoy/immt_a_chs/WebviewActivity;

    invoke-virtual {v1, v2, v3}, Landroid/content/Intent;->setClass(Landroid/content/Context;Ljava/lang/Class;)Landroid/content/Intent;

    .line 281
    sget-object v2, Lcom/locojoy/PurchaseHandler;->context:Landroid/content/Context;

    invoke-virtual {v2, v1}, Landroid/content/Context;->startActivity(Landroid/content/Intent;)V

    .line 326
    return-void
.end method

.method protected showLoading(Ljava/lang/String;)V
    .locals 2
    .parameter "tips"

    .prologue
    .line 196
    new-instance v0, Landroid/app/ProgressDialog;

    sget-object v1, Lcom/locojoy/PurchaseHandler;->context:Landroid/content/Context;

    invoke-direct {v0, v1}, Landroid/app/ProgressDialog;-><init>(Landroid/content/Context;)V

    iput-object v0, p0, Lcom/locojoy/PurchaseHandler;->progressDialog:Landroid/app/ProgressDialog;

    .line 197
    iget-object v0, p0, Lcom/locojoy/PurchaseHandler;->progressDialog:Landroid/app/ProgressDialog;

    invoke-virtual {v0, p1}, Landroid/app/ProgressDialog;->setMessage(Ljava/lang/CharSequence;)V

    .line 198
    iget-object v0, p0, Lcom/locojoy/PurchaseHandler;->progressDialog:Landroid/app/ProgressDialog;

    const/4 v1, 0x1

    invoke-virtual {v0, v1}, Landroid/app/ProgressDialog;->setIndeterminate(Z)V

    .line 199
    iget-object v0, p0, Lcom/locojoy/PurchaseHandler;->progressDialog:Landroid/app/ProgressDialog;

    const/4 v1, 0x0

    invoke-virtual {v0, v1}, Landroid/app/ProgressDialog;->setCancelable(Z)V

    .line 200
    iget-object v0, p0, Lcom/locojoy/PurchaseHandler;->progressDialog:Landroid/app/ProgressDialog;

    invoke-virtual {v0}, Landroid/app/ProgressDialog;->show()V

    .line 201
    return-void
.end method

.method public updateVersion()V
    .locals 0

    .prologue
    .line 216
    return-void
.end method
