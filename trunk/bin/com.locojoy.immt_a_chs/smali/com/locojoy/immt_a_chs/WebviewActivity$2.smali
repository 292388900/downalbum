.class Lcom/locojoy/immt_a_chs/WebviewActivity$2;
.super Ljava/lang/Object;
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
    iput-object p1, p0, Lcom/locojoy/immt_a_chs/WebviewActivity$2;->this$0:Lcom/locojoy/immt_a_chs/WebviewActivity;

    .line 70
    invoke-direct {p0}, Ljava/lang/Object;-><init>()V

    return-void
.end method

.method static synthetic access$0(Lcom/locojoy/immt_a_chs/WebviewActivity$2;)Lcom/locojoy/immt_a_chs/WebviewActivity;
    .locals 1
    .parameter

    .prologue
    .line 70
    iget-object v0, p0, Lcom/locojoy/immt_a_chs/WebviewActivity$2;->this$0:Lcom/locojoy/immt_a_chs/WebviewActivity;

    return-object v0
.end method


# virtual methods
.method public clickOnAndroid()V
    .locals 2

    .prologue
    .line 74
    iget-object v0, p0, Lcom/locojoy/immt_a_chs/WebviewActivity$2;->this$0:Lcom/locojoy/immt_a_chs/WebviewActivity;

    iget-object v0, v0, Lcom/locojoy/immt_a_chs/WebviewActivity;->handler:Landroid/os/Handler;

    .line 75
    new-instance v1, Lcom/locojoy/immt_a_chs/WebviewActivity$2$1;

    invoke-direct {v1, p0}, Lcom/locojoy/immt_a_chs/WebviewActivity$2$1;-><init>(Lcom/locojoy/immt_a_chs/WebviewActivity$2;)V

    .line 74
    invoke-virtual {v0, v1}, Landroid/os/Handler;->post(Ljava/lang/Runnable;)Z

    .line 83
    return-void
.end method
