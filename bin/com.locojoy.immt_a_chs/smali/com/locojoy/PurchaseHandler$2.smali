.class Lcom/locojoy/PurchaseHandler$2;
.super Ljava/lang/Thread;
.source "PurchaseHandler.java"


# annotations
.annotation system Ldalvik/annotation/EnclosingMethod;
    value = Lcom/locojoy/PurchaseHandler;->handleMessage(Landroid/os/Message;)V
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
    iput-object p1, p0, Lcom/locojoy/PurchaseHandler$2;->this$0:Lcom/locojoy/PurchaseHandler;

    .line 60
    invoke-direct {p0}, Ljava/lang/Thread;-><init>()V

    return-void
.end method


# virtual methods
.method public run()V
    .locals 1

    .prologue
    .line 70
    iget-object v0, p0, Lcom/locojoy/PurchaseHandler$2;->this$0:Lcom/locojoy/PurchaseHandler;

    #calls: Lcom/locojoy/PurchaseHandler;->accountLogin()V
    invoke-static {v0}, Lcom/locojoy/PurchaseHandler;->access$1(Lcom/locojoy/PurchaseHandler;)V

    .line 71
    return-void
.end method
