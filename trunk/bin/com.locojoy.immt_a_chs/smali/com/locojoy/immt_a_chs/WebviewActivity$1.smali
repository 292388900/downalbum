.class Lcom/locojoy/immt_a_chs/WebviewActivity$1;
.super Landroid/webkit/WebViewClient;
.source "WebviewActivity.java"


# annotations
.annotation system Ldalvik/annotation/EnclosingMethod;
    value = Lcom/locojoy/immt_a_chs/WebviewActivity;->onCreate(Landroid/os/Bundle;)V
.end annotation

.annotation system Ldalvik/annotation/InnerClass;
    accessFlags = 0x0
    name = null
.end annotation


# instance fields
.field final synthetic this$0:Lcom/locojoy/immt_a_chs/WebviewActivity;


# direct methods
.method constructor <init>(Lcom/locojoy/immt_a_chs/WebviewActivity;)V
    .locals 0
    .parameter

    .prologue
    .line 1
    iput-object p1, p0, Lcom/locojoy/immt_a_chs/WebviewActivity$1;->this$0:Lcom/locojoy/immt_a_chs/WebviewActivity;

    .line 49
    invoke-direct {p0}, Landroid/webkit/WebViewClient;-><init>()V

    return-void
.end method


# virtual methods
.method public shouldOverrideUrlLoading(Landroid/webkit/WebView;Ljava/lang/String;)Z
    .locals 6
    .parameter "view"
    .parameter "url"

    .prologue
    const/4 v5, 0x1

    const/4 v3, 0x0

    .line 54
    invoke-static {}, Lcom/locojoy/common/LocojoyCommonMethod;->getLocojoyPayCheckUrl1()Ljava/lang/String;

    move-result-object v0

    .line 55
    .local v0, payCheckUrl1:Ljava/lang/String;
    invoke-static {}, Lcom/locojoy/common/LocojoyCommonMethod;->getLocojoyPayCheckUrl2()Ljava/lang/String;

    move-result-object v1

    .line 56
    .local v1, payCheckUrl2:Ljava/lang/String;
    invoke-virtual {p2, v0, v3}, Ljava/lang/String;->indexOf(Ljava/lang/String;I)I

    move-result v2

    if-gez v2, :cond_0

    invoke-virtual {p2, v1, v3}, Ljava/lang/String;->indexOf(Ljava/lang/String;I)I

    move-result v2

    if-ltz v2, :cond_1

    .line 58
    :cond_0
    const-string v2, "payCheckUrl----"

    new-instance v3, Ljava/lang/StringBuilder;

    const-string v4, "url1="

    invoke-direct {v3, v4}, Ljava/lang/StringBuilder;-><init>(Ljava/lang/String;)V

    invoke-virtual {v3, v0}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    move-result-object v3

    const-string v4, "url2="

    invoke-virtual {v3, v4}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    move-result-object v3

    invoke-virtual {v3, v1}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    move-result-object v3

    const-string v4, "url="

    invoke-virtual {v3, v4}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    move-result-object v3

    invoke-virtual {v3, p2}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    move-result-object v3

    invoke-virtual {v3}, Ljava/lang/StringBuilder;->toString()Ljava/lang/String;

    move-result-object v3

    invoke-static {v2, v3}, Landroid/util/Log;->e(Ljava/lang/String;Ljava/lang/String;)I

    .line 59
    iget-object v2, p0, Lcom/locojoy/immt_a_chs/WebviewActivity$1;->this$0:Lcom/locojoy/immt_a_chs/WebviewActivity;

    #setter for: Lcom/locojoy/immt_a_chs/WebviewActivity;->isValidFlag:Z
    invoke-static {v2, v5}, Lcom/locojoy/immt_a_chs/WebviewActivity;->access$0(Lcom/locojoy/immt_a_chs/WebviewActivity;Z)V

    .line 61
    :cond_1
    invoke-virtual {p1, p2}, Landroid/webkit/WebView;->loadUrl(Ljava/lang/String;)V

    .line 62
    return v5
.end method
