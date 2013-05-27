.class Lcom/locojoy/PurchaseHandler$1;
.super Ljava/lang/Object;
.source "PurchaseHandler.java"

# interfaces
.implements Ljava/lang/Runnable;


# annotations
.annotation system Ldalvik/annotation/EnclosingMethod;
    value = Lcom/locojoy/PurchaseHandler;-><init>(Landroid/content/Context;)V
.end annotation

.annotation system Ldalvik/annotation/InnerClass;
    accessFlags = 0x0
    name = null
.end annotation


# instance fields
.field final synthetic this$0:Lcom/locojoy/PurchaseHandler;


# direct methods
.method constructor <init>(Lcom/locojoy/PurchaseHandler;)V
    .locals 0
    .parameter

    .prologue
    .line 1
    iput-object p1, p0, Lcom/locojoy/PurchaseHandler$1;->this$0:Lcom/locojoy/PurchaseHandler;

    .line 41
    invoke-direct {p0}, Ljava/lang/Object;-><init>()V

    return-void
.end method


# virtual methods
.method public run()V
    .locals 1

    .prologue
    .line 45
    iget-object v0, p0, Lcom/locojoy/PurchaseHandler$1;->this$0:Lcom/locojoy/PurchaseHandler;

    #calls: Lcom/locojoy/PurchaseHandler;->initSDK()V
    invoke-static {v0}, Lcom/locojoy/PurchaseHandler;->access$0(Lcom/locojoy/PurchaseHandler;)V

    .line 46
    iget-object v0, p0, Lcom/locojoy/PurchaseHandler$1;->this$0:Lcom/locojoy/PurchaseHandler;

    invoke-virtual {v0}, Lcom/locojoy/PurchaseHandler;->appVersionUpdateDidFinish()V

    .line 47
    return-void
.end method
