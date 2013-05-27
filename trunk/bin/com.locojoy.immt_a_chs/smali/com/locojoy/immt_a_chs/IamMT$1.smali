.class Lcom/locojoy/immt_a_chs/IamMT$1;
.super Ljava/lang/Object;
.source "IamMT.java"

# interfaces
.implements Landroid/content/DialogInterface$OnClickListener;


# annotations
.annotation system Ldalvik/annotation/EnclosingMethod;
    value = Lcom/locojoy/immt_a_chs/IamMT;->checkDevice()V
.end annotation

.annotation system Ldalvik/annotation/InnerClass;
    accessFlags = 0x0
    name = null
.end annotation


# instance fields
.field final synthetic this$0:Lcom/locojoy/immt_a_chs/IamMT;


# direct methods
.method constructor <init>(Lcom/locojoy/immt_a_chs/IamMT;)V
    .locals 0
    .parameter

    .prologue
    .line 1
    iput-object p1, p0, Lcom/locojoy/immt_a_chs/IamMT$1;->this$0:Lcom/locojoy/immt_a_chs/IamMT;

    .line 227
    invoke-direct {p0}, Ljava/lang/Object;-><init>()V

    return-void
.end method


# virtual methods
.method public onClick(Landroid/content/DialogInterface;I)V
    .locals 1
    .parameter "arg0"
    .parameter "arg1"

    .prologue
    .line 232
    iget-object v0, p0, Lcom/locojoy/immt_a_chs/IamMT$1;->this$0:Lcom/locojoy/immt_a_chs/IamMT;

    invoke-virtual {v0}, Lcom/locojoy/immt_a_chs/IamMT;->finish()V

    .line 233
    const/4 v0, 0x0

    invoke-static {v0}, Ljava/lang/System;->exit(I)V

    .line 234
    return-void
.end method
