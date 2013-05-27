.class public Lcom/locojoy/immt_a_chs/WebviewActivity;
.super Landroid/app/Activity;
.source "WebviewActivity.java"


# instance fields
.field private CurValidOrder:Ljava/lang/String;

.field private CurValidProduct_id:Ljava/lang/String;

.field handler:Landroid/os/Handler;

.field private isValidFlag:Z

.field purchView:Landroid/webkit/WebView;


# direct methods
.method public constructor <init>()V
    .locals 1

    .prologue
    .line 19
    invoke-direct {p0}, Landroid/app/Activity;-><init>()V

    .line 20
    const/4 v0, 0x0

    iput-boolean v0, p0, Lcom/locojoy/immt_a_chs/WebviewActivity;->isValidFlag:Z

    .line 25
    new-instance v0, Landroid/os/Handler;

    invoke-direct {v0}, Landroid/os/Handler;-><init>()V

    iput-object v0, p0, Lcom/locojoy/immt_a_chs/WebviewActivity;->handler:Landroid/os/Handler;

    .line 19
    return-void
.end method

.method static synthetic access$0(Lcom/locojoy/immt_a_chs/WebviewActivity;Z)V
    .locals 0
    .parameter
    .parameter

    .prologue
    .line 20
    iput-boolean p1, p0, Lcom/locojoy/immt_a_chs/WebviewActivity;->isValidFlag:Z

    return-void
.end method

.method static synthetic access$1(Lcom/locojoy/immt_a_chs/WebviewActivity;)Z
    .locals 1
    .parameter

    .prologue
    .line 20
    iget-boolean v0, p0, Lcom/locojoy/immt_a_chs/WebviewActivity;->isValidFlag:Z

    return v0
.end method

.method static synthetic access$2(Lcom/locojoy/immt_a_chs/WebviewActivity;)Ljava/lang/String;
    .locals 1
    .parameter

    .prologue
    .line 22
    iget-object v0, p0, Lcom/locojoy/immt_a_chs/WebviewActivity;->CurValidOrder:Ljava/lang/String;

    return-object v0
.end method

.method static synthetic access$3(Lcom/locojoy/immt_a_chs/WebviewActivity;)Ljava/lang/String;
    .locals 1
    .parameter

    .prologue
    .line 21
    iget-object v0, p0, Lcom/locojoy/immt_a_chs/WebviewActivity;->CurValidProduct_id:Ljava/lang/String;

    return-object v0
.end method

.method static synthetic access$4(Lcom/locojoy/immt_a_chs/WebviewActivity;Ljava/lang/String;)V
    .locals 0
    .parameter
    .parameter

    .prologue
    .line 22
    iput-object p1, p0, Lcom/locojoy/immt_a_chs/WebviewActivity;->CurValidOrder:Ljava/lang/String;

    return-void
.end method

.method static synthetic access$5(Lcom/locojoy/immt_a_chs/WebviewActivity;Ljava/lang/String;)V
    .locals 0
    .parameter
    .parameter

    .prologue
    .line 21
    iput-object p1, p0, Lcom/locojoy/immt_a_chs/WebviewActivity;->CurValidProduct_id:Ljava/lang/String;

    return-void
.end method


# virtual methods
.method protected onCreate(Landroid/os/Bundle;)V
    .locals 7
    .parameter "savedInstanceState"

    .prologue
    .line 32
    invoke-super {p0, p1}, Landroid/app/Activity;->onCreate(Landroid/os/Bundle;)V

    .line 33
    const/high16 v4, 0x7f03

    invoke-virtual {p0, v4}, Lcom/locojoy/immt_a_chs/WebviewActivity;->setContentView(I)V

    .line 34
    invoke-virtual {p0}, Lcom/locojoy/immt_a_chs/WebviewActivity;->getIntent()Landroid/content/Intent;

    move-result-object v2

    .line 35
    .local v2, intent:Landroid/content/Intent;
    invoke-virtual {v2}, Landroid/content/Intent;->getExtras()Landroid/os/Bundle;

    move-result-object v0

    .line 36
    .local v0, bundle:Landroid/os/Bundle;
    const-string v4, "product_id"

    invoke-virtual {v0, v4}, Landroid/os/Bundle;->getString(Ljava/lang/String;)Ljava/lang/String;

    move-result-object v4

    iput-object v4, p0, Lcom/locojoy/immt_a_chs/WebviewActivity;->CurValidProduct_id:Ljava/lang/String;

    .line 37
    const-string v4, "order"

    invoke-virtual {v0, v4}, Landroid/os/Bundle;->getString(Ljava/lang/String;)Ljava/lang/String;

    move-result-object v4

    iput-object v4, p0, Lcom/locojoy/immt_a_chs/WebviewActivity;->CurValidOrder:Ljava/lang/String;

    .line 40
    const/high16 v4, 0x7f05

    invoke-virtual {p0, v4}, Lcom/locojoy/immt_a_chs/WebviewActivity;->findViewById(I)Landroid/view/View;

    move-result-object v4

    check-cast v4, Landroid/webkit/WebView;

    iput-object v4, p0, Lcom/locojoy/immt_a_chs/WebviewActivity;->purchView:Landroid/webkit/WebView;

    .line 42
    iget-object v4, p0, Lcom/locojoy/immt_a_chs/WebviewActivity;->purchView:Landroid/webkit/WebView;

    invoke-virtual {v4}, Landroid/webkit/WebView;->getSettings()Landroid/webkit/WebSettings;

    move-result-object v3

    .line 43
    .local v3, setting:Landroid/webkit/WebSettings;
    const/4 v4, 0x1

    invoke-virtual {v3, v4}, Landroid/webkit/WebSettings;->setJavaScriptEnabled(Z)V

    .line 44
    iget-object v4, p0, Lcom/locojoy/immt_a_chs/WebviewActivity;->purchView:Landroid/webkit/WebView;

    invoke-virtual {v4}, Landroid/webkit/WebView;->requestFocus()Z

    .line 45
    iget-object v4, p0, Lcom/locojoy/immt_a_chs/WebviewActivity;->purchView:Landroid/webkit/WebView;

    const/high16 v5, 0x200

    invoke-virtual {v4, v5}, Landroid/webkit/WebView;->setScrollBarStyle(I)V

    .line 48
    iget-object v4, p0, Lcom/locojoy/immt_a_chs/WebviewActivity;->purchView:Landroid/webkit/WebView;

    .line 49
    new-instance v5, Lcom/locojoy/immt_a_chs/WebviewActivity$1;

    invoke-direct {v5, p0}, Lcom/locojoy/immt_a_chs/WebviewActivity$1;-><init>(Lcom/locojoy/immt_a_chs/WebviewActivity;)V

    .line 48
    invoke-virtual {v4, v5}, Landroid/webkit/WebView;->setWebViewClient(Landroid/webkit/WebViewClient;)V

    .line 69
    iget-object v4, p0, Lcom/locojoy/immt_a_chs/WebviewActivity;->purchView:Landroid/webkit/WebView;

    .line 70
    new-instance v5, Lcom/locojoy/immt_a_chs/WebviewActivity$2;

    invoke-direct {v5, p0}, Lcom/locojoy/immt_a_chs/WebviewActivity$2;-><init>(Lcom/locojoy/immt_a_chs/WebviewActivity;)V

    .line 85
    const-string v6, "demo"

    .line 69
    invoke-virtual {v4, v5, v6}, Landroid/webkit/WebView;->addJavascriptInterface(Ljava/lang/Object;Ljava/lang/String;)V

    .line 88
    iget-object v4, p0, Lcom/locojoy/immt_a_chs/WebviewActivity;->purchView:Landroid/webkit/WebView;

    iget-object v5, p0, Lcom/locojoy/immt_a_chs/WebviewActivity;->CurValidProduct_id:Ljava/lang/String;

    .line 89
    iget-object v6, p0, Lcom/locojoy/immt_a_chs/WebviewActivity;->CurValidOrder:Ljava/lang/String;

    .line 88
    invoke-static {v5, v6}, Lcom/locojoy/common/LocojoyCommonMethod;->getLocojoyPayUrl(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;

    move-result-object v5

    invoke-virtual {v4, v5}, Landroid/webkit/WebView;->loadUrl(Ljava/lang/String;)V

    .line 92
    const v4, 0x7f050001

    invoke-virtual {p0, v4}, Lcom/locojoy/immt_a_chs/WebviewActivity;->findViewById(I)Landroid/view/View;

    move-result-object v1

    check-cast v1, Landroid/widget/Button;

    .line 94
    .local v1, button:Landroid/widget/Button;
    new-instance v4, Lcom/locojoy/immt_a_chs/WebviewActivity$3;

    invoke-direct {v4, p0}, Lcom/locojoy/immt_a_chs/WebviewActivity$3;-><init>(Lcom/locojoy/immt_a_chs/WebviewActivity;)V

    .line 93
    invoke-virtual {v1, v4}, Landroid/widget/Button;->setOnClickListener(Landroid/view/View$OnClickListener;)V

    .line 118
    return-void
.end method
