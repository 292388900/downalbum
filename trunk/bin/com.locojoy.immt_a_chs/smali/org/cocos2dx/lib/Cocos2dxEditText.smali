.class public Lorg/cocos2dx/lib/Cocos2dxEditText;
.super Landroid/widget/EditText;
.source "Cocos2dxEditText.java"


# instance fields
.field private mView:Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;


# direct methods
.method public constructor <init>(Landroid/content/Context;)V
    .locals 0
    .parameter "context"

    .prologue
    .line 37
    invoke-direct {p0, p1}, Landroid/widget/EditText;-><init>(Landroid/content/Context;)V

    .line 39
    return-void
.end method

.method public constructor <init>(Landroid/content/Context;Landroid/util/AttributeSet;)V
    .locals 0
    .parameter "context"
    .parameter "attrs"

    .prologue
    .line 42
    invoke-direct {p0, p1, p2}, Landroid/widget/EditText;-><init>(Landroid/content/Context;Landroid/util/AttributeSet;)V

    .line 43
    return-void
.end method

.method public constructor <init>(Landroid/content/Context;Landroid/util/AttributeSet;I)V
    .locals 0
    .parameter "context"
    .parameter "attrs"
    .parameter "defStyle"

    .prologue
    .line 46
    invoke-direct {p0, p1, p2, p3}, Landroid/widget/EditText;-><init>(Landroid/content/Context;Landroid/util/AttributeSet;I)V

    .line 47
    return-void
.end method


# virtual methods
.method public onKeyDown(ILandroid/view/KeyEvent;)Z
    .locals 1
    .parameter "keyCode"
    .parameter "event"

    .prologue
    .line 57
    invoke-super {p0, p1, p2}, Landroid/widget/EditText;->onKeyDown(ILandroid/view/KeyEvent;)Z

    .line 59
    const/4 v0, 0x4

    if-ne p1, v0, :cond_0

    .line 60
    iget-object v0, p0, Lorg/cocos2dx/lib/Cocos2dxEditText;->mView:Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;

    invoke-virtual {v0}, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;->requestFocus()Z

    .line 63
    :cond_0
    const/4 v0, 0x1

    return v0
.end method

.method public setMainView(Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;)V
    .locals 0
    .parameter "glSurfaceView"

    .prologue
    .line 50
    iput-object p1, p0, Lorg/cocos2dx/lib/Cocos2dxEditText;->mView:Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;

    .line 51
    return-void
.end method
