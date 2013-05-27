.class Lcom/locojoy/immt_a_chs/WebviewActivity$2$1;
.super Ljava/lang/Object;
.source "WebviewActivity.java"

# interfaces
.implements Ljava/lang/Runnable;


# annotations
.annotation system Ldalvik/annotation/EnclosingMethod;
    value = Lcom/locojoy/immt_a_chs/WebviewActivity$2;->clickOnAndroid()V
.end annotation

.annotation system Ldalvik/annotation/InnerClass;
    accessFlags = 0x0
    name = null
.end annotation


# instance fields
.field final synthetic this$1:Lcom/locojoy/immt_a_chs/WebviewActivity$2;


# direct methods
.method constructor <init>(Lcom/locojoy/immt_a_chs/WebviewActivity$2;)V
    .locals 0
    .parameter

    .prologue
    .line 1
    iput-object p1, p0, Lcom/locojoy/immt_a_chs/WebviewActivity$2$1;->this$1:Lcom/locojoy/immt_a_chs/WebviewActivity$2;

    .line 75
    invoke-direct {p0}, Ljava/lang/Object;-><init>()V

    return-void
.end method


# virtual methods
.method public run()V
    .locals 2

    .prologue
    .line 79
    iget-object v0, p0, Lcom/locojoy/immt_a_chs/WebviewActivity$2$1;->this$1:Lcom/locojoy/immt_a_chs/WebviewActivity$2;

    #getter for: Lcom/locojoy/immt_a_chs/WebviewActivity$2;->this$0:Lcom/locojoy/immt_a_chs/WebviewActivity;
    invoke-static {v0}, Lcom/locojoy/immt_a_chs/WebviewActivity$2;->access$0(Lcom/locojoy/immt_a_chs/WebviewActivity$2;)Lcom/locojoy/immt_a_chs/WebviewActivity;

    move-result-object v0

    iget-object v0, v0, Lcom/locojoy/immt_a_chs/WebviewActivity;->purchView:Landroid/webkit/WebView;

    const-string v1, "javascript:wave()"

    invoke-virtual {v0, v1}, Landroid/webkit/WebView;->loadUrl(Ljava/lang/String;)V

    .line 80
    return-void
.end method
