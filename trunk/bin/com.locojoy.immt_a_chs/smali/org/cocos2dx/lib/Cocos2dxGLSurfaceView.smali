.class public Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;
.super Landroid/opengl/GLSurfaceView;
.source "Cocos2dxGLSurfaceView.java"


# static fields
.field private static final HANDLER_CLOSE_IME_KEYBOARD:I = 0x3

.field private static final HANDLER_OPEN_IME_KEYBOARD:I = 0x2

.field private static final TAG:Ljava/lang/String;

.field private static final debug:Z

.field private static handler:Landroid/os/Handler;

.field private static mainView:Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;

.field private static textInputWraper:Lorg/cocos2dx/lib/TextInputWraper;


# instance fields
.field private mRenderer:Lorg/cocos2dx/lib/Cocos2dxRenderer;

.field private mTextField:Lorg/cocos2dx/lib/Cocos2dxEditText;


# direct methods
.method static constructor <clinit>()V
    .locals 1

    .prologue
    .line 134
    const-class v0, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;

    invoke-virtual {v0}, Ljava/lang/Class;->getCanonicalName()Ljava/lang/String;

    move-result-object v0

    sput-object v0, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;->TAG:Ljava/lang/String;

    .line 222
    return-void
.end method

.method public constructor <init>(Landroid/content/Context;)V
    .locals 0
    .parameter "context"

    .prologue
    .line 143
    invoke-direct {p0, p1}, Landroid/opengl/GLSurfaceView;-><init>(Landroid/content/Context;)V

    .line 144
    invoke-virtual {p0}, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;->initView()V

    .line 145
    return-void
.end method

.method public constructor <init>(Landroid/content/Context;Landroid/util/AttributeSet;)V
    .locals 0
    .parameter "context"
    .parameter "attrs"

    .prologue
    .line 148
    invoke-direct {p0, p1, p2}, Landroid/opengl/GLSurfaceView;-><init>(Landroid/content/Context;Landroid/util/AttributeSet;)V

    .line 149
    invoke-virtual {p0}, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;->initView()V

    .line 150
    return-void
.end method

.method static synthetic access$0(Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;)Lorg/cocos2dx/lib/Cocos2dxEditText;
    .locals 1
    .parameter

    .prologue
    .line 223
    iget-object v0, p0, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;->mTextField:Lorg/cocos2dx/lib/Cocos2dxEditText;

    return-object v0
.end method

.method static synthetic access$1()Lorg/cocos2dx/lib/TextInputWraper;
    .locals 1

    .prologue
    .line 222
    sget-object v0, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;->textInputWraper:Lorg/cocos2dx/lib/TextInputWraper;

    return-object v0
.end method

.method static synthetic access$2()Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;
    .locals 1

    .prologue
    .line 132
    sget-object v0, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;->mainView:Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;

    return-object v0
.end method

.method static synthetic access$3(Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;)Lorg/cocos2dx/lib/Cocos2dxRenderer;
    .locals 1
    .parameter

    .prologue
    .line 135
    iget-object v0, p0, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;->mRenderer:Lorg/cocos2dx/lib/Cocos2dxRenderer;

    return-object v0
.end method

.method public static closeIMEKeyboard()V
    .locals 2

    .prologue
    .line 251
    new-instance v0, Landroid/os/Message;

    invoke-direct {v0}, Landroid/os/Message;-><init>()V

    .line 252
    .local v0, msg:Landroid/os/Message;
    const/4 v1, 0x3

    iput v1, v0, Landroid/os/Message;->what:I

    .line 253
    sget-object v1, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;->handler:Landroid/os/Handler;

    invoke-virtual {v1, v0}, Landroid/os/Handler;->sendMessage(Landroid/os/Message;)Z

    .line 254
    return-void
.end method

.method private dumpEvent(Landroid/view/MotionEvent;)V
    .locals 9
    .parameter "event"

    .prologue
    const/4 v8, 0x6

    const/4 v7, 0x5

    .line 439
    const/16 v5, 0xa

    new-array v3, v5, [Ljava/lang/String;

    const/4 v5, 0x0

    const-string v6, "DOWN"

    aput-object v6, v3, v5

    const/4 v5, 0x1

    const-string v6, "UP"

    aput-object v6, v3, v5

    const/4 v5, 0x2

    const-string v6, "MOVE"

    aput-object v6, v3, v5

    const/4 v5, 0x3

    const-string v6, "CANCEL"

    aput-object v6, v3, v5

    const/4 v5, 0x4

    const-string v6, "OUTSIDE"

    aput-object v6, v3, v5

    .line 440
    const-string v5, "POINTER_DOWN"

    aput-object v5, v3, v7

    const-string v5, "POINTER_UP"

    aput-object v5, v3, v8

    const/4 v5, 0x7

    const-string v6, "7?"

    aput-object v6, v3, v5

    const/16 v5, 0x8

    const-string v6, "8?"

    aput-object v6, v3, v5

    const/16 v5, 0x9

    const-string v6, "9?"

    aput-object v6, v3, v5

    .line 441
    .local v3, names:[Ljava/lang/String;
    new-instance v4, Ljava/lang/StringBuilder;

    invoke-direct {v4}, Ljava/lang/StringBuilder;-><init>()V

    .line 442
    .local v4, sb:Ljava/lang/StringBuilder;
    invoke-virtual {p1}, Landroid/view/MotionEvent;->getAction()I

    move-result v0

    .line 443
    .local v0, action:I
    and-int/lit16 v1, v0, 0xff

    .line 444
    .local v1, actionCode:I
    const-string v5, "event ACTION_"

    invoke-virtual {v4, v5}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    move-result-object v5

    aget-object v6, v3, v1

    invoke-virtual {v5, v6}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    .line 445
    if-eq v1, v7, :cond_0

    .line 446
    if-ne v1, v8, :cond_1

    .line 447
    :cond_0
    const-string v5, "(pid "

    invoke-virtual {v4, v5}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    move-result-object v5

    .line 448
    shr-int/lit8 v6, v0, 0x8

    .line 447
    invoke-virtual {v5, v6}, Ljava/lang/StringBuilder;->append(I)Ljava/lang/StringBuilder;

    .line 449
    const-string v5, ")"

    invoke-virtual {v4, v5}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    .line 451
    :cond_1
    const-string v5, "["

    invoke-virtual {v4, v5}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    .line 452
    const/4 v2, 0x0

    .local v2, i:I
    :goto_0
    invoke-virtual {p1}, Landroid/view/MotionEvent;->getPointerCount()I

    move-result v5

    if-lt v2, v5, :cond_2

    .line 460
    const-string v5, "]"

    invoke-virtual {v4, v5}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    .line 461
    sget-object v5, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;->TAG:Ljava/lang/String;

    invoke-virtual {v4}, Ljava/lang/StringBuilder;->toString()Ljava/lang/String;

    move-result-object v6

    invoke-static {v5, v6}, Landroid/util/Log;->d(Ljava/lang/String;Ljava/lang/String;)I

    .line 462
    return-void

    .line 453
    :cond_2
    const-string v5, "#"

    invoke-virtual {v4, v5}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    move-result-object v5

    invoke-virtual {v5, v2}, Ljava/lang/StringBuilder;->append(I)Ljava/lang/StringBuilder;

    .line 454
    const-string v5, "(pid "

    invoke-virtual {v4, v5}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    move-result-object v5

    invoke-virtual {p1, v2}, Landroid/view/MotionEvent;->getPointerId(I)I

    move-result v6

    invoke-virtual {v5, v6}, Ljava/lang/StringBuilder;->append(I)Ljava/lang/StringBuilder;

    .line 455
    const-string v5, ")="

    invoke-virtual {v4, v5}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    move-result-object v5

    invoke-virtual {p1, v2}, Landroid/view/MotionEvent;->getX(I)F

    move-result v6

    float-to-int v6, v6

    invoke-virtual {v5, v6}, Ljava/lang/StringBuilder;->append(I)Ljava/lang/StringBuilder;

    .line 456
    const-string v5, ","

    invoke-virtual {v4, v5}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    move-result-object v5

    invoke-virtual {p1, v2}, Landroid/view/MotionEvent;->getY(I)F

    move-result v6

    float-to-int v6, v6

    invoke-virtual {v5, v6}, Ljava/lang/StringBuilder;->append(I)Ljava/lang/StringBuilder;

    .line 457
    add-int/lit8 v5, v2, 0x1

    invoke-virtual {p1}, Landroid/view/MotionEvent;->getPointerCount()I

    move-result v6

    if-ge v5, v6, :cond_3

    .line 458
    const-string v5, ";"

    invoke-virtual {v4, v5}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    .line 452
    :cond_3
    add-int/lit8 v2, v2, 0x1

    goto :goto_0
.end method

.method private getContentText()Ljava/lang/String;
    .locals 1

    .prologue
    .line 247
    iget-object v0, p0, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;->mRenderer:Lorg/cocos2dx/lib/Cocos2dxRenderer;

    invoke-virtual {v0}, Lorg/cocos2dx/lib/Cocos2dxRenderer;->getContentText()Ljava/lang/String;

    move-result-object v0

    return-object v0
.end method

.method public static openIMEKeyboard()V
    .locals 2

    .prologue
    .line 239
    new-instance v0, Landroid/os/Message;

    invoke-direct {v0}, Landroid/os/Message;-><init>()V

    .line 240
    .local v0, msg:Landroid/os/Message;
    const/4 v1, 0x2

    iput v1, v0, Landroid/os/Message;->what:I

    .line 241
    sget-object v1, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;->mainView:Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;

    invoke-direct {v1}, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;->getContentText()Ljava/lang/String;

    move-result-object v1

    iput-object v1, v0, Landroid/os/Message;->obj:Ljava/lang/Object;

    .line 242
    sget-object v1, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;->handler:Landroid/os/Handler;

    invoke-virtual {v1, v0}, Landroid/os/Handler;->sendMessage(Landroid/os/Message;)Z

    .line 244
    return-void
.end method


# virtual methods
.method public deleteBackward()V
    .locals 1

    .prologue
    .line 266
    new-instance v0, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView$5;

    invoke-direct {v0, p0}, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView$5;-><init>(Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;)V

    invoke-virtual {p0, v0}, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;->queueEvent(Ljava/lang/Runnable;)V

    .line 272
    return-void
.end method

.method public getTextField()Landroid/widget/TextView;
    .locals 1

    .prologue
    .line 226
    iget-object v0, p0, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;->mTextField:Lorg/cocos2dx/lib/Cocos2dxEditText;

    return-object v0
.end method

.method protected initView()V
    .locals 1

    .prologue
    .line 158
    const/4 v0, 0x1

    invoke-virtual {p0, v0}, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;->setFocusableInTouchMode(Z)V

    .line 160
    new-instance v0, Lorg/cocos2dx/lib/TextInputWraper;

    invoke-direct {v0, p0}, Lorg/cocos2dx/lib/TextInputWraper;-><init>(Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;)V

    sput-object v0, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;->textInputWraper:Lorg/cocos2dx/lib/TextInputWraper;

    .line 162
    new-instance v0, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView$1;

    invoke-direct {v0, p0}, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView$1;-><init>(Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;)V

    sput-object v0, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;->handler:Landroid/os/Handler;

    .line 191
    sput-object p0, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;->mainView:Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;

    .line 192
    return-void
.end method

.method public insertText(Ljava/lang/String;)V
    .locals 1
    .parameter "text"

    .prologue
    .line 257
    new-instance v0, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView$4;

    invoke-direct {v0, p0, p1}, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView$4;-><init>(Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;Ljava/lang/String;)V

    invoke-virtual {p0, v0}, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;->queueEvent(Ljava/lang/Runnable;)V

    .line 263
    return-void
.end method

.method public onKeyDown(ILandroid/view/KeyEvent;)Z
    .locals 8
    .parameter "keyCode"
    .parameter "event"

    .prologue
    const/4 v5, 0x1

    .line 391
    move v2, p1

    .line 392
    .local v2, kc:I
    const/16 v6, 0x52

    if-ne p1, v6, :cond_0

    .line 394
    new-instance v6, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView$12;

    invoke-direct {v6, p0, v2}, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView$12;-><init>(Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;I)V

    invoke-virtual {p0, v6}, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;->queueEvent(Ljava/lang/Runnable;)V

    .line 434
    :goto_0
    return v5

    .line 402
    :cond_0
    const/4 v6, 0x4

    if-ne p1, v6, :cond_1

    .line 404
    invoke-virtual {p0}, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;->getContext()Landroid/content/Context;

    move-result-object v6

    const v7, 0x7f040009

    invoke-virtual {v6, v7}, Landroid/content/Context;->getString(I)Ljava/lang/String;

    move-result-object v4

    .line 405
    .local v4, title:Ljava/lang/String;
    invoke-virtual {p0}, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;->getContext()Landroid/content/Context;

    move-result-object v6

    const v7, 0x7f04000a

    invoke-virtual {v6, v7}, Landroid/content/Context;->getString(I)Ljava/lang/String;

    move-result-object v1

    .line 406
    .local v1, content:Ljava/lang/String;
    invoke-virtual {p0}, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;->getContext()Landroid/content/Context;

    move-result-object v6

    const v7, 0x7f04000b

    invoke-virtual {v6, v7}, Landroid/content/Context;->getString(I)Ljava/lang/String;

    move-result-object v3

    .line 407
    .local v3, ok:Ljava/lang/String;
    invoke-virtual {p0}, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;->getContext()Landroid/content/Context;

    move-result-object v6

    const v7, 0x7f04000c

    invoke-virtual {v6, v7}, Landroid/content/Context;->getString(I)Ljava/lang/String;

    move-result-object v0

    .line 409
    .local v0, cancel:Ljava/lang/String;
    new-instance v6, Landroid/app/AlertDialog$Builder;

    invoke-virtual {p0}, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;->getContext()Landroid/content/Context;

    move-result-object v7

    invoke-direct {v6, v7}, Landroid/app/AlertDialog$Builder;-><init>(Landroid/content/Context;)V

    invoke-virtual {v6, v4}, Landroid/app/AlertDialog$Builder;->setTitle(Ljava/lang/CharSequence;)Landroid/app/AlertDialog$Builder;

    move-result-object v6

    .line 410
    invoke-virtual {v6, v1}, Landroid/app/AlertDialog$Builder;->setMessage(Ljava/lang/CharSequence;)Landroid/app/AlertDialog$Builder;

    move-result-object v6

    .line 411
    new-instance v7, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView$13;

    invoke-direct {v7, p0}, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView$13;-><init>(Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;)V

    invoke-virtual {v6, v3, v7}, Landroid/app/AlertDialog$Builder;->setPositiveButton(Ljava/lang/CharSequence;Landroid/content/DialogInterface$OnClickListener;)Landroid/app/AlertDialog$Builder;

    move-result-object v6

    .line 417
    new-instance v7, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView$14;

    invoke-direct {v7, p0}, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView$14;-><init>(Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;)V

    invoke-virtual {v6, v0, v7}, Landroid/app/AlertDialog$Builder;->setNegativeButton(Ljava/lang/CharSequence;Landroid/content/DialogInterface$OnClickListener;)Landroid/app/AlertDialog$Builder;

    move-result-object v6

    .line 423
    invoke-virtual {v6}, Landroid/app/AlertDialog$Builder;->show()Landroid/app/AlertDialog;

    .line 426
    new-instance v6, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView$15;

    invoke-direct {v6, p0, v2}, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView$15;-><init>(Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;I)V

    invoke-virtual {p0, v6}, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;->queueEvent(Ljava/lang/Runnable;)V

    goto :goto_0

    .line 434
    .end local v0           #cancel:Ljava/lang/String;
    .end local v1           #content:Ljava/lang/String;
    .end local v3           #ok:Ljava/lang/String;
    .end local v4           #title:Ljava/lang/String;
    :cond_1
    invoke-super {p0, p1, p2}, Landroid/opengl/GLSurfaceView;->onKeyDown(ILandroid/view/KeyEvent;)Z

    move-result v5

    goto :goto_0
.end method

.method public onPause()V
    .locals 1

    .prologue
    .line 195
    new-instance v0, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView$2;

    invoke-direct {v0, p0}, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView$2;-><init>(Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;)V

    invoke-virtual {p0, v0}, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;->queueEvent(Ljava/lang/Runnable;)V

    .line 202
    invoke-super {p0}, Landroid/opengl/GLSurfaceView;->onPause()V

    .line 203
    return-void
.end method

.method public onResume()V
    .locals 1

    .prologue
    .line 206
    invoke-super {p0}, Landroid/opengl/GLSurfaceView;->onResume()V

    .line 208
    new-instance v0, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView$3;

    invoke-direct {v0, p0}, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView$3;-><init>(Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;)V

    invoke-virtual {p0, v0}, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;->queueEvent(Ljava/lang/Runnable;)V

    .line 214
    return-void
.end method

.method protected onSizeChanged(IIII)V
    .locals 1
    .parameter "w"
    .parameter "h"
    .parameter "oldw"
    .parameter "oldh"

    .prologue
    .line 386
    iget-object v0, p0, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;->mRenderer:Lorg/cocos2dx/lib/Cocos2dxRenderer;

    invoke-virtual {v0, p1, p2}, Lorg/cocos2dx/lib/Cocos2dxRenderer;->setScreenWidthAndHeight(II)V

    .line 387
    return-void
.end method

.method public onTouchEvent(Landroid/view/MotionEvent;)Z
    .locals 24
    .parameter "event"

    .prologue
    .line 280
    invoke-virtual/range {p1 .. p1}, Landroid/view/MotionEvent;->getPointerCount()I

    move-result v12

    .line 282
    .local v12, pointerNumber:I
    const/16 v23, 0x1

    move/from16 v0, v23

    if-le v12, v0, :cond_0

    .line 283
    const/16 v23, 0x0

    .line 379
    :goto_0
    return v23

    .line 285
    :cond_0
    new-array v9, v12, [I

    .line 286
    .local v9, ids:[I
    new-array v0, v12, [F

    move-object/from16 v17, v0

    .line 287
    .local v17, xs:[F
    new-array v0, v12, [F

    move-object/from16 v22, v0

    .line 289
    .local v22, ys:[F
    const/4 v4, 0x0

    .local v4, i:I
    :goto_1
    if-lt v4, v12, :cond_1

    .line 295
    invoke-virtual/range {p1 .. p1}, Landroid/view/MotionEvent;->getAction()I

    move-result v23

    move/from16 v0, v23

    and-int/lit16 v0, v0, 0xff

    move/from16 v23, v0

    packed-switch v23, :pswitch_data_0

    .line 379
    :goto_2
    :pswitch_0
    const/16 v23, 0x1

    goto :goto_0

    .line 290
    :cond_1
    move-object/from16 v0, p1

    invoke-virtual {v0, v4}, Landroid/view/MotionEvent;->getPointerId(I)I

    move-result v23

    aput v23, v9, v4

    .line 291
    move-object/from16 v0, p1

    invoke-virtual {v0, v4}, Landroid/view/MotionEvent;->getX(I)F

    move-result v23

    aput v23, v17, v4

    .line 292
    move-object/from16 v0, p1

    invoke-virtual {v0, v4}, Landroid/view/MotionEvent;->getY(I)F

    move-result v23

    aput v23, v22, v4

    .line 289
    add-int/lit8 v4, v4, 0x1

    goto :goto_1

    .line 297
    :pswitch_1
    invoke-virtual/range {p1 .. p1}, Landroid/view/MotionEvent;->getAction()I

    move-result v23

    shr-int/lit8 v11, v23, 0x8

    .line 298
    .local v11, indexPointerDown:I
    move-object/from16 v0, p1

    invoke-virtual {v0, v11}, Landroid/view/MotionEvent;->getPointerId(I)I

    move-result v6

    .line 299
    .local v6, idPointerDown:I
    move-object/from16 v0, p1

    invoke-virtual {v0, v11}, Landroid/view/MotionEvent;->getX(I)F

    move-result v14

    .line 300
    .local v14, xPointerDown:F
    move-object/from16 v0, p1

    invoke-virtual {v0, v11}, Landroid/view/MotionEvent;->getY(I)F

    move-result v19

    .line 302
    .local v19, yPointerDown:F
    new-instance v23, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView$6;

    move-object/from16 v0, v23

    move-object/from16 v1, p0

    move/from16 v2, v19

    invoke-direct {v0, v1, v6, v14, v2}, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView$6;-><init>(Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;IFF)V

    move-object/from16 v0, p0

    move-object/from16 v1, v23

    invoke-virtual {v0, v1}, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;->queueEvent(Ljava/lang/Runnable;)V

    goto :goto_2

    .line 312
    .end local v6           #idPointerDown:I
    .end local v11           #indexPointerDown:I
    .end local v14           #xPointerDown:F
    .end local v19           #yPointerDown:F
    :pswitch_2
    const/16 v23, 0x0

    move-object/from16 v0, p1

    move/from16 v1, v23

    invoke-virtual {v0, v1}, Landroid/view/MotionEvent;->getPointerId(I)I

    move-result v5

    .line 313
    .local v5, idDown:I
    const/16 v23, 0x0

    aget v13, v17, v23

    .line 314
    .local v13, xDown:F
    const/16 v23, 0x0

    aget v18, v22, v23

    .line 316
    .local v18, yDown:F
    new-instance v23, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView$7;

    move-object/from16 v0, v23

    move-object/from16 v1, p0

    move/from16 v2, v18

    invoke-direct {v0, v1, v5, v13, v2}, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView$7;-><init>(Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;IFF)V

    move-object/from16 v0, p0

    move-object/from16 v1, v23

    invoke-virtual {v0, v1}, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;->queueEvent(Ljava/lang/Runnable;)V

    goto :goto_2

    .line 325
    .end local v5           #idDown:I
    .end local v13           #xDown:F
    .end local v18           #yDown:F
    :pswitch_3
    new-instance v23, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView$8;

    move-object/from16 v0, v23

    move-object/from16 v1, p0

    move-object/from16 v2, v17

    move-object/from16 v3, v22

    invoke-direct {v0, v1, v9, v2, v3}, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView$8;-><init>(Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;[I[F[F)V

    move-object/from16 v0, p0

    move-object/from16 v1, v23

    invoke-virtual {v0, v1}, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;->queueEvent(Ljava/lang/Runnable;)V

    goto/16 :goto_2

    .line 339
    :pswitch_4
    invoke-virtual/range {p1 .. p1}, Landroid/view/MotionEvent;->getAction()I

    move-result v23

    shr-int/lit8 v10, v23, 0x8

    .line 340
    .local v10, indexPointUp:I
    move-object/from16 v0, p1

    invoke-virtual {v0, v10}, Landroid/view/MotionEvent;->getPointerId(I)I

    move-result v7

    .line 341
    .local v7, idPointerUp:I
    move-object/from16 v0, p1

    invoke-virtual {v0, v10}, Landroid/view/MotionEvent;->getX(I)F

    move-result v15

    .line 342
    .local v15, xPointerUp:F
    move-object/from16 v0, p1

    invoke-virtual {v0, v10}, Landroid/view/MotionEvent;->getY(I)F

    move-result v20

    .line 344
    .local v20, yPointerUp:F
    new-instance v23, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView$9;

    move-object/from16 v0, v23

    move-object/from16 v1, p0

    move/from16 v2, v20

    invoke-direct {v0, v1, v7, v15, v2}, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView$9;-><init>(Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;IFF)V

    move-object/from16 v0, p0

    move-object/from16 v1, v23

    invoke-virtual {v0, v1}, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;->queueEvent(Ljava/lang/Runnable;)V

    goto/16 :goto_2

    .line 354
    .end local v7           #idPointerUp:I
    .end local v10           #indexPointUp:I
    .end local v15           #xPointerUp:F
    .end local v20           #yPointerUp:F
    :pswitch_5
    const/16 v23, 0x0

    move-object/from16 v0, p1

    move/from16 v1, v23

    invoke-virtual {v0, v1}, Landroid/view/MotionEvent;->getPointerId(I)I

    move-result v8

    .line 355
    .local v8, idUp:I
    const/16 v23, 0x0

    aget v16, v17, v23

    .line 356
    .local v16, xUp:F
    const/16 v23, 0x0

    aget v21, v22, v23

    .line 358
    .local v21, yUp:F
    new-instance v23, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView$10;

    move-object/from16 v0, v23

    move-object/from16 v1, p0

    move/from16 v2, v16

    move/from16 v3, v21

    invoke-direct {v0, v1, v8, v2, v3}, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView$10;-><init>(Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;IFF)V

    move-object/from16 v0, p0

    move-object/from16 v1, v23

    invoke-virtual {v0, v1}, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;->queueEvent(Ljava/lang/Runnable;)V

    goto/16 :goto_2

    .line 367
    .end local v8           #idUp:I
    .end local v16           #xUp:F
    .end local v21           #yUp:F
    :pswitch_6
    new-instance v23, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView$11;

    move-object/from16 v0, v23

    move-object/from16 v1, p0

    move-object/from16 v2, v17

    move-object/from16 v3, v22

    invoke-direct {v0, v1, v9, v2, v3}, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView$11;-><init>(Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;[I[F[F)V

    move-object/from16 v0, p0

    move-object/from16 v1, v23

    invoke-virtual {v0, v1}, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;->queueEvent(Ljava/lang/Runnable;)V

    goto/16 :goto_2

    .line 295
    nop

    :pswitch_data_0
    .packed-switch 0x0
        :pswitch_2
        :pswitch_5
        :pswitch_3
        :pswitch_6
        :pswitch_0
        :pswitch_1
        :pswitch_4
    .end packed-switch
.end method

.method public setCocos2dxRenderer(Lorg/cocos2dx/lib/Cocos2dxRenderer;)V
    .locals 1
    .parameter "renderer"

    .prologue
    .line 153
    iput-object p1, p0, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;->mRenderer:Lorg/cocos2dx/lib/Cocos2dxRenderer;

    .line 154
    iget-object v0, p0, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;->mRenderer:Lorg/cocos2dx/lib/Cocos2dxRenderer;

    invoke-virtual {p0, v0}, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;->setRenderer(Landroid/opengl/GLSurfaceView$Renderer;)V

    .line 155
    return-void
.end method

.method public setTextField(Lorg/cocos2dx/lib/Cocos2dxEditText;)V
    .locals 2
    .parameter "view"

    .prologue
    .line 230
    iput-object p1, p0, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;->mTextField:Lorg/cocos2dx/lib/Cocos2dxEditText;

    .line 231
    iget-object v0, p0, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;->mTextField:Lorg/cocos2dx/lib/Cocos2dxEditText;

    if-eqz v0, :cond_0

    sget-object v0, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;->textInputWraper:Lorg/cocos2dx/lib/TextInputWraper;

    if-eqz v0, :cond_0

    .line 232
    iget-object v0, p0, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;->mTextField:Lorg/cocos2dx/lib/Cocos2dxEditText;

    sget-object v1, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;->textInputWraper:Lorg/cocos2dx/lib/TextInputWraper;

    invoke-virtual {v0, v1}, Lorg/cocos2dx/lib/Cocos2dxEditText;->setOnEditorActionListener(Landroid/widget/TextView$OnEditorActionListener;)V

    .line 233
    iget-object v0, p0, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;->mTextField:Lorg/cocos2dx/lib/Cocos2dxEditText;

    invoke-virtual {v0, p0}, Lorg/cocos2dx/lib/Cocos2dxEditText;->setMainView(Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;)V

    .line 234
    invoke-virtual {p0}, Lorg/cocos2dx/lib/Cocos2dxGLSurfaceView;->requestFocus()Z

    .line 236
    :cond_0
    return-void
.end method
