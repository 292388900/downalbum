.class Lcom/locojoy/immt_a_chs/WebviewActivity$3;
.super Ljava/lang/Object;
.source "WebviewActivity.java"

# interfaces
.implements Landroid/view/View$OnClickListener;


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
    iput-object p1, p0, Lcom/locojoy/immt_a_chs/WebviewActivity$3;->this$0:Lcom/locojoy/immt_a_chs/WebviewActivity;

    .line 94
    invoke-direct {p0}, Ljava/lang/Object;-><init>()V

    return-void
.end method


# virtual methods
.method public onClick(Landroid/view/View;)V
    .locals 4
    .parameter "v"

    .prologue
    const/4 v3, 0x0

    .line 100
    iget-object v0, p0, Lcom/locojoy/immt_a_chs/WebviewActivity$3;->this$0:Lcom/locojoy/immt_a_chs/WebviewActivity;

    #getter for: Lcom/locojoy/immt_a_chs/WebviewActivity;->isValidFlag:Z
    invoke-static {v0}, Lcom/locojoy/immt_a_chs/WebviewActivity;->access$1(Lcom/locojoy/immt_a_chs/WebviewActivity;)Z

    move-result v0

    if-eqz v0, :cond_0

    iget-object v0, p0, Lcom/locojoy/immt_a_chs/WebviewActivity$3;->this$0:Lcom/locojoy/immt_a_chs/WebviewActivity;

    #getter for: Lcom/locojoy/immt_a_chs/WebviewActivity;->CurValidOrder:Ljava/lang/String;
    invoke-static {v0}, Lcom/locojoy/immt_a_chs/WebviewActivity;->access$2(Lcom/locojoy/immt_a_chs/WebviewActivity;)Ljava/lang/String;

    move-result-object v0

    const-string v1, ""

    if-eq v0, v1, :cond_0

    iget-object v0, p0, Lcom/locojoy/immt_a_chs/WebviewActivity$3;->this$0:Lcom/locojoy/immt_a_chs/WebviewActivity;

    #getter for: Lcom/locojoy/immt_a_chs/WebviewActivity;->CurValidProduct_id:Ljava/lang/String;
    invoke-static {v0}, Lcom/locojoy/immt_a_chs/WebviewActivity;->access$3(Lcom/locojoy/immt_a_chs/WebviewActivity;)Ljava/lang/String;

    move-result-object v0

    const-string v1, ""

    if-eq v0, v1, :cond_0

    .line 102
    iget-object v0, p0, Lcom/locojoy/immt_a_chs/WebviewActivity$3;->this$0:Lcom/locojoy/immt_a_chs/WebviewActivity;

    #getter for: Lcom/locojoy/immt_a_chs/WebviewActivity;->CurValidOrder:Ljava/lang/String;
    invoke-static {v0}, Lcom/locojoy/immt_a_chs/WebviewActivity;->access$2(Lcom/locojoy/immt_a_chs/WebviewActivity;)Ljava/lang/String;

    move-result-object v0

    iget-object v1, p0, Lcom/locojoy/immt_a_chs/WebviewActivity$3;->this$0:Lcom/locojoy/immt_a_chs/WebviewActivity;

    #getter for: Lcom/locojoy/immt_a_chs/WebviewActivity;->CurValidProduct_id:Ljava/lang/String;
    invoke-static {v1}, Lcom/locojoy/immt_a_chs/WebviewActivity;->access$3(Lcom/locojoy/immt_a_chs/WebviewActivity;)Ljava/lang/String;

    move-result-object v1

    const/4 v2, 0x1

    invoke-static {v0, v1, v2}, Lcom/locojoy/PurchaseHandler;->UniPayAsynResult(Ljava/lang/String;Ljava/lang/String;I)V

    .line 103
    iget-object v0, p0, Lcom/locojoy/immt_a_chs/WebviewActivity$3;->this$0:Lcom/locojoy/immt_a_chs/WebviewActivity;

    const-string v1, ""

    #setter for: Lcom/locojoy/immt_a_chs/WebviewActivity;->CurValidOrder:Ljava/lang/String;
    invoke-static {v0, v1}, Lcom/locojoy/immt_a_chs/WebviewActivity;->access$4(Lcom/locojoy/immt_a_chs/WebviewActivity;Ljava/lang/String;)V

    .line 104
    iget-object v0, p0, Lcom/locojoy/immt_a_chs/WebviewActivity$3;->this$0:Lcom/locojoy/immt_a_chs/WebviewActivity;

    const-string v1, ""

    #setter for: Lcom/locojoy/immt_a_chs/WebviewActivity;->CurValidProduct_id:Ljava/lang/String;
    invoke-static {v0, v1}, Lcom/locojoy/immt_a_chs/WebviewActivity;->access$5(Lcom/locojoy/immt_a_chs/WebviewActivity;Ljava/lang/String;)V

    .line 105
    iget-object v0, p0, Lcom/locojoy/immt_a_chs/WebviewActivity$3;->this$0:Lcom/locojoy/immt_a_chs/WebviewActivity;

    #setter for: Lcom/locojoy/immt_a_chs/WebviewActivity;->isValidFlag:Z
    invoke-static {v0, v3}, Lcom/locojoy/immt_a_chs/WebviewActivity;->access$0(Lcom/locojoy/immt_a_chs/WebviewActivity;Z)V

    .line 113
    :goto_0
    iget-object v0, p0, Lcom/locojoy/immt_a_chs/WebviewActivity$3;->this$0:Lcom/locojoy/immt_a_chs/WebviewActivity;

    invoke-virtual {v0}, Lcom/locojoy/immt_a_chs/WebviewActivity;->finish()V

    .line 114
    return-void

    .line 109
    :cond_0
    iget-object v0, p0, Lcom/locojoy/immt_a_chs/WebviewActivity$3;->this$0:Lcom/locojoy/immt_a_chs/WebviewActivity;

    #getter for: Lcom/locojoy/immt_a_chs/WebviewActivity;->CurValidOrder:Ljava/lang/String;
    invoke-static {v0}, Lcom/locojoy/immt_a_chs/WebviewActivity;->access$2(Lcom/locojoy/immt_a_chs/WebviewActivity;)Ljava/lang/String;

    move-result-object v0

    iget-object v1, p0, Lcom/locojoy/immt_a_chs/WebviewActivity$3;->this$0:Lcom/locojoy/immt_a_chs/WebviewActivity;

    #getter for: Lcom/locojoy/immt_a_chs/WebviewActivity;->CurValidProduct_id:Ljava/lang/String;
    invoke-static {v1}, Lcom/locojoy/immt_a_chs/WebviewActivity;->access$3(Lcom/locojoy/immt_a_chs/WebviewActivity;)Ljava/lang/String;

    move-result-object v1

    invoke-static {v0, v1, v3}, Lcom/locojoy/PurchaseHandler;->UniPayAsynResult(Ljava/lang/String;Ljava/lang/String;I)V

    .line 110
    iget-object v0, p0, Lcom/locojoy/immt_a_chs/WebviewActivity$3;->this$0:Lcom/locojoy/immt_a_chs/WebviewActivity;

    const-string v1, ""

    #setter for: Lcom/locojoy/immt_a_chs/WebviewActivity;->CurValidOrder:Ljava/lang/String;
    invoke-static {v0, v1}, Lcom/locojoy/immt_a_chs/WebviewActivity;->access$4(Lcom/locojoy/immt_a_chs/WebviewActivity;Ljava/lang/String;)V

    .line 111
    iget-object v0, p0, Lcom/locojoy/immt_a_chs/WebviewActivity$3;->this$0:Lcom/locojoy/immt_a_chs/WebviewActivity;

    const-string v1, ""

    #setter for: Lcom/locojoy/immt_a_chs/WebviewActivity;->CurValidProduct_id:Ljava/lang/String;
    invoke-static {v0, v1}, Lcom/locojoy/immt_a_chs/WebviewActivity;->access$5(Lcom/locojoy/immt_a_chs/WebviewActivity;Ljava/lang/String;)V

    goto :goto_0
.end method
