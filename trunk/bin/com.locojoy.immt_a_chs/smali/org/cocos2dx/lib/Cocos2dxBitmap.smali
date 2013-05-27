.class public Lorg/cocos2dx/lib/Cocos2dxBitmap;
.super Ljava/lang/Object;
.source "Cocos2dxBitmap.java"


# annotations
.annotation system Ldalvik/annotation/MemberClasses;
    value = {
        Lorg/cocos2dx/lib/Cocos2dxBitmap$TextProperty;
    }
.end annotation


# static fields
.field private static final HALIGNCENTER:I = 0x3

.field private static final HALIGNLEFT:I = 0x1

.field private static final HALIGNRIGHT:I = 0x2

.field private static final VALIGNBOTTOM:I = 0x2

.field private static final VALIGNCENTER:I = 0x3

.field private static final VALIGNTOP:I = 0x1

.field private static context:Landroid/content/Context;


# direct methods
.method public constructor <init>()V
    .locals 0

    .prologue
    .line 40
    invoke-direct {p0}, Ljava/lang/Object;-><init>()V

    return-void
.end method

.method private static computeTextProperty(Ljava/lang/String;Landroid/graphics/Paint;II)Lorg/cocos2dx/lib/Cocos2dxBitmap$TextProperty;
    .locals 11
    .parameter "content"
    .parameter "paint"
    .parameter "maxWidth"
    .parameter "maxHeight"

    .prologue
    const/4 v7, 0x0

    .line 161
    invoke-virtual {p1}, Landroid/graphics/Paint;->getFontMetricsInt()Landroid/graphics/Paint$FontMetricsInt;

    move-result-object v0

    .line 162
    .local v0, fm:Landroid/graphics/Paint$FontMetricsInt;
    iget v6, v0, Landroid/graphics/Paint$FontMetricsInt;->bottom:I

    iget v8, v0, Landroid/graphics/Paint$FontMetricsInt;->top:I

    sub-int/2addr v6, v8

    int-to-double v8, v6

    invoke-static {v8, v9}, Ljava/lang/Math;->ceil(D)D

    move-result-wide v8

    double-to-int v1, v8

    .line 163
    .local v1, h:I
    const/4 v4, 0x0

    .line 165
    .local v4, maxContentWidth:I
    invoke-static {p0, p3, p2, p1}, Lorg/cocos2dx/lib/Cocos2dxBitmap;->splitString(Ljava/lang/String;IILandroid/graphics/Paint;)[Ljava/lang/String;

    move-result-object v3

    .line 167
    .local v3, lines:[Ljava/lang/String;
    if-eqz p2, :cond_1

    .line 168
    move v4, p2

    .line 183
    :cond_0
    new-instance v6, Lorg/cocos2dx/lib/Cocos2dxBitmap$TextProperty;

    invoke-direct {v6, v4, v1, v3}, Lorg/cocos2dx/lib/Cocos2dxBitmap$TextProperty;-><init>(II[Ljava/lang/String;)V

    return-object v6

    .line 174
    :cond_1
    const/4 v5, 0x0

    .line 175
    .local v5, temp:I
    array-length v8, v3

    move v6, v7

    :goto_0
    if-ge v6, v8, :cond_0

    aget-object v2, v3, v6

    .line 176
    .local v2, line:Ljava/lang/String;
    invoke-virtual {v2}, Ljava/lang/String;->length()I

    move-result v9

    invoke-virtual {p1, v2, v7, v9}, Landroid/graphics/Paint;->measureText(Ljava/lang/String;II)F

    move-result v9

    float-to-double v9, v9

    invoke-static {v9, v10}, Ljava/lang/Math;->ceil(D)D

    move-result-wide v9

    double-to-int v5, v9

    .line 177
    if-le v5, v4, :cond_2

    .line 178
    move v4, v5

    .line 175
    :cond_2
    add-int/lit8 v6, v6, 0x1

    goto :goto_0
.end method

.method private static computeX(Landroid/graphics/Paint;Ljava/lang/String;II)I
    .locals 2
    .parameter "paint"
    .parameter "content"
    .parameter "w"
    .parameter "alignment"

    .prologue
    .line 92
    const/4 v1, 0x0

    .line 93
    .local v1, ret:I
    and-int/lit8 v0, p3, 0xf

    .line 95
    .local v0, hAlignment:I
    packed-switch v0, :pswitch_data_0

    .line 116
    :goto_0
    :pswitch_0
    return v1

    .line 97
    :pswitch_1
    div-int/lit8 v1, p2, 0x2

    .line 98
    goto :goto_0

    .line 105
    :pswitch_2
    move v1, p2

    .line 106
    goto :goto_0

    .line 95
    :pswitch_data_0
    .packed-switch 0x1
        :pswitch_0
        :pswitch_2
        :pswitch_1
    .end packed-switch
.end method

.method private static computeY(Landroid/graphics/Paint$FontMetricsInt;III)I
    .locals 4
    .parameter "fm"
    .parameter "constrainHeight"
    .parameter "totalHeight"
    .parameter "alignment"

    .prologue
    .line 120
    iget v2, p0, Landroid/graphics/Paint$FontMetricsInt;->top:I

    neg-int v1, v2

    .line 122
    .local v1, y:I
    if-le p1, p2, :cond_0

    .line 123
    shr-int/lit8 v2, p3, 0x4

    and-int/lit8 v0, v2, 0xf

    .line 125
    .local v0, vAlignment:I
    packed-switch v0, :pswitch_data_0

    .line 140
    .end local v0           #vAlignment:I
    :cond_0
    :goto_0
    return v1

    .line 127
    .restart local v0       #vAlignment:I
    :pswitch_0
    iget v2, p0, Landroid/graphics/Paint$FontMetricsInt;->top:I

    neg-int v1, v2

    .line 128
    goto :goto_0

    .line 130
    :pswitch_1
    iget v2, p0, Landroid/graphics/Paint$FontMetricsInt;->top:I

    neg-int v2, v2

    sub-int v3, p1, p2

    div-int/lit8 v3, v3, 0x2

    add-int v1, v2, v3

    .line 131
    goto :goto_0

    .line 133
    :pswitch_2
    iget v2, p0, Landroid/graphics/Paint$FontMetricsInt;->top:I

    neg-int v2, v2

    sub-int v3, p1, p2

    add-int v1, v2, v3

    .line 134
    goto :goto_0

    .line 125
    nop

    :pswitch_data_0
    .packed-switch 0x1
        :pswitch_0
        :pswitch_2
        :pswitch_1
    .end packed-switch
.end method

.method public static createTextBitmap(Ljava/lang/String;Ljava/lang/String;IIII)V
    .locals 17
    .parameter "content"
    .parameter "fontName"
    .parameter "fontSize"
    .parameter "alignment"
    .parameter "width"
    .parameter "height"

    .prologue
    .line 65
    invoke-static/range {p0 .. p0}, Lorg/cocos2dx/lib/Cocos2dxBitmap;->refactorString(Ljava/lang/String;)Ljava/lang/String;

    move-result-object p0

    .line 66
    invoke-static/range {p1 .. p3}, Lorg/cocos2dx/lib/Cocos2dxBitmap;->newPaint(Ljava/lang/String;II)Landroid/graphics/Paint;

    move-result-object v9

    .line 68
    .local v9, paint:Landroid/graphics/Paint;
    move-object/from16 v0, p0

    move/from16 v1, p4

    move/from16 v2, p5

    invoke-static {v0, v9, v1, v2}, Lorg/cocos2dx/lib/Cocos2dxBitmap;->computeTextProperty(Ljava/lang/String;Landroid/graphics/Paint;II)Lorg/cocos2dx/lib/Cocos2dxBitmap$TextProperty;

    move-result-object v10

    .line 70
    .local v10, textProperty:Lorg/cocos2dx/lib/Cocos2dxBitmap$TextProperty;
    if-nez p5, :cond_0

    iget v4, v10, Lorg/cocos2dx/lib/Cocos2dxBitmap$TextProperty;->totalHeight:I

    .line 73
    .local v4, bitmapTotalHeight:I
    :goto_0
    iget v13, v10, Lorg/cocos2dx/lib/Cocos2dxBitmap$TextProperty;->maxWidth:I

    .line 74
    sget-object v14, Landroid/graphics/Bitmap$Config;->ARGB_8888:Landroid/graphics/Bitmap$Config;

    .line 73
    invoke-static {v13, v4, v14}, Landroid/graphics/Bitmap;->createBitmap(IILandroid/graphics/Bitmap$Config;)Landroid/graphics/Bitmap;

    move-result-object v3

    .line 75
    .local v3, bitmap:Landroid/graphics/Bitmap;
    new-instance v5, Landroid/graphics/Canvas;

    invoke-direct {v5, v3}, Landroid/graphics/Canvas;-><init>(Landroid/graphics/Bitmap;)V

    .line 78
    .local v5, canvas:Landroid/graphics/Canvas;
    invoke-virtual {v9}, Landroid/graphics/Paint;->getFontMetricsInt()Landroid/graphics/Paint$FontMetricsInt;

    move-result-object v6

    .line 79
    .local v6, fm:Landroid/graphics/Paint$FontMetricsInt;
    const/4 v11, 0x0

    .line 80
    .local v11, x:I
    iget v13, v10, Lorg/cocos2dx/lib/Cocos2dxBitmap$TextProperty;->totalHeight:I

    move/from16 v0, p5

    move/from16 v1, p3

    invoke-static {v6, v0, v13, v1}, Lorg/cocos2dx/lib/Cocos2dxBitmap;->computeY(Landroid/graphics/Paint$FontMetricsInt;III)I

    move-result v12

    .line 81
    .local v12, y:I
    iget-object v8, v10, Lorg/cocos2dx/lib/Cocos2dxBitmap$TextProperty;->lines:[Ljava/lang/String;

    .line 82
    .local v8, lines:[Ljava/lang/String;
    array-length v14, v8

    const/4 v13, 0x0

    :goto_1
    if-lt v13, v14, :cond_1

    .line 88
    invoke-static {v3}, Lorg/cocos2dx/lib/Cocos2dxBitmap;->initNativeObject(Landroid/graphics/Bitmap;)V

    .line 89
    return-void

    .end local v3           #bitmap:Landroid/graphics/Bitmap;
    .end local v4           #bitmapTotalHeight:I
    .end local v5           #canvas:Landroid/graphics/Canvas;
    .end local v6           #fm:Landroid/graphics/Paint$FontMetricsInt;
    .end local v8           #lines:[Ljava/lang/String;
    .end local v11           #x:I
    .end local v12           #y:I
    :cond_0
    move/from16 v4, p5

    .line 70
    goto :goto_0

    .line 82
    .restart local v3       #bitmap:Landroid/graphics/Bitmap;
    .restart local v4       #bitmapTotalHeight:I
    .restart local v5       #canvas:Landroid/graphics/Canvas;
    .restart local v6       #fm:Landroid/graphics/Paint$FontMetricsInt;
    .restart local v8       #lines:[Ljava/lang/String;
    .restart local v11       #x:I
    .restart local v12       #y:I
    :cond_1
    aget-object v7, v8, v13

    .line 83
    .local v7, line:Ljava/lang/String;
    iget v15, v10, Lorg/cocos2dx/lib/Cocos2dxBitmap$TextProperty;->maxWidth:I

    move/from16 v0, p3

    invoke-static {v9, v7, v15, v0}, Lorg/cocos2dx/lib/Cocos2dxBitmap;->computeX(Landroid/graphics/Paint;Ljava/lang/String;II)I

    move-result v11

    .line 84
    int-to-float v15, v11

    int-to-float v0, v12

    move/from16 v16, v0

    move/from16 v0, v16

    invoke-virtual {v5, v7, v15, v0, v9}, Landroid/graphics/Canvas;->drawText(Ljava/lang/String;FFLandroid/graphics/Paint;)V

    .line 85
    iget v15, v10, Lorg/cocos2dx/lib/Cocos2dxBitmap$TextProperty;->heightPerLine:I

    add-int/2addr v12, v15

    .line 82
    add-int/lit8 v13, v13, 0x1

    goto :goto_1
.end method

.method private static divideStringWithMaxWidth(Landroid/graphics/Paint;Ljava/lang/String;I)Ljava/util/LinkedList;
    .locals 9
    .parameter "paint"
    .parameter "content"
    .parameter "width"
    .annotation system Ldalvik/annotation/Signature;
        value = {
            "(",
            "Landroid/graphics/Paint;",
            "Ljava/lang/String;",
            "I)",
            "Ljava/util/LinkedList",
            "<",
            "Ljava/lang/String;",
            ">;"
        }
    .end annotation

    .prologue
    .line 253
    invoke-virtual {p1}, Ljava/lang/String;->length()I

    move-result v0

    .line 254
    .local v0, charLength:I
    const/4 v4, 0x0

    .line 255
    .local v4, start:I
    const/4 v6, 0x0

    .line 256
    .local v6, tempWidth:I
    new-instance v5, Ljava/util/LinkedList;

    invoke-direct {v5}, Ljava/util/LinkedList;-><init>()V

    .line 261
    .local v5, strList:Ljava/util/LinkedList;,"Ljava/util/LinkedList<Ljava/lang/String;>;"
    const/4 v1, 0x1

    .local v1, i:I
    :goto_0
    if-le v1, v0, :cond_1

    .line 300
    if-ge v4, v0, :cond_0

    .line 301
    invoke-virtual {p1, v4}, Ljava/lang/String;->substring(I)Ljava/lang/String;

    move-result-object v7

    invoke-virtual {v5, v7}, Ljava/util/LinkedList;->add(Ljava/lang/Object;)Z

    .line 304
    :cond_0
    return-object v5

    .line 262
    :cond_1
    invoke-virtual {p0, p1, v4, v1}, Landroid/graphics/Paint;->measureText(Ljava/lang/String;II)F

    move-result v7

    float-to-double v7, v7

    invoke-static {v7, v8}, Ljava/lang/Math;->ceil(D)D

    move-result-wide v7

    double-to-int v6, v7

    .line 263
    if-lt v6, p2, :cond_2

    .line 264
    const/4 v7, 0x0

    invoke-virtual {p1, v7, v1}, Ljava/lang/String;->substring(II)Ljava/lang/String;

    move-result-object v7

    const-string v8, " "

    invoke-virtual {v7, v8}, Ljava/lang/String;->lastIndexOf(Ljava/lang/String;)I

    move-result v3

    .line 266
    .local v3, lastIndexOfSpace:I
    const/4 v7, -0x1

    if-eq v3, v7, :cond_3

    if-le v3, v4, :cond_3

    .line 270
    invoke-virtual {p1, v4, v3}, Ljava/lang/String;->substring(II)Ljava/lang/String;

    move-result-object v7

    invoke-virtual {v5, v7}, Ljava/util/LinkedList;->add(Ljava/lang/Object;)Z

    .line 271
    move v1, v3

    .line 290
    :goto_1
    add-int/lit8 v2, v1, 0x1

    .end local v1           #i:I
    .local v2, i:I
    invoke-virtual {p1, v1}, Ljava/lang/String;->indexOf(I)I

    move-result v7

    const/16 v8, 0x20

    if-eq v7, v8, :cond_5

    .line 293
    move v4, v2

    move v1, v2

    .line 261
    .end local v2           #i:I
    .end local v3           #lastIndexOfSpace:I
    .restart local v1       #i:I
    :cond_2
    add-int/lit8 v1, v1, 0x1

    goto :goto_0

    .line 277
    .restart local v3       #lastIndexOfSpace:I
    :cond_3
    if-le v6, p2, :cond_4

    .line 278
    add-int/lit8 v7, v1, -0x1

    invoke-virtual {p1, v4, v7}, Ljava/lang/String;->substring(II)Ljava/lang/String;

    move-result-object v7

    invoke-virtual {v5, v7}, Ljava/util/LinkedList;->add(Ljava/lang/Object;)Z

    .line 282
    add-int/lit8 v1, v1, -0x1

    .line 283
    goto :goto_1

    .line 285
    :cond_4
    invoke-virtual {p1, v4, v1}, Ljava/lang/String;->substring(II)Ljava/lang/String;

    move-result-object v7

    invoke-virtual {v5, v7}, Ljava/util/LinkedList;->add(Ljava/lang/Object;)Z

    goto :goto_1

    .end local v1           #i:I
    .restart local v2       #i:I
    :cond_5
    move v1, v2

    .end local v2           #i:I
    .restart local v1       #i:I
    goto :goto_1
.end method

.method private static getPixels(Landroid/graphics/Bitmap;)[B
    .locals 4
    .parameter "bitmap"

    .prologue
    .line 401
    if-eqz p0, :cond_0

    .line 402
    invoke-virtual {p0}, Landroid/graphics/Bitmap;->getWidth()I

    move-result v2

    invoke-virtual {p0}, Landroid/graphics/Bitmap;->getHeight()I

    move-result v3

    mul-int/2addr v2, v3

    mul-int/lit8 v2, v2, 0x4

    new-array v1, v2, [B

    .line 403
    .local v1, pixels:[B
    invoke-static {v1}, Ljava/nio/ByteBuffer;->wrap([B)Ljava/nio/ByteBuffer;

    move-result-object v0

    .line 404
    .local v0, buf:Ljava/nio/ByteBuffer;
    invoke-static {}, Ljava/nio/ByteOrder;->nativeOrder()Ljava/nio/ByteOrder;

    move-result-object v2

    invoke-virtual {v0, v2}, Ljava/nio/ByteBuffer;->order(Ljava/nio/ByteOrder;)Ljava/nio/ByteBuffer;

    .line 405
    invoke-virtual {p0, v0}, Landroid/graphics/Bitmap;->copyPixelsToBuffer(Ljava/nio/Buffer;)V

    .line 409
    .end local v0           #buf:Ljava/nio/ByteBuffer;
    .end local v1           #pixels:[B
    :goto_0
    return-object v1

    :cond_0
    const/4 v1, 0x0

    goto :goto_0
.end method

.method private static initNativeObject(Landroid/graphics/Bitmap;)V
    .locals 3
    .parameter "bitmap"

    .prologue
    .line 392
    invoke-static {p0}, Lorg/cocos2dx/lib/Cocos2dxBitmap;->getPixels(Landroid/graphics/Bitmap;)[B

    move-result-object v0

    .line 393
    .local v0, pixels:[B
    if-nez v0, :cond_0

    .line 398
    :goto_0
    return-void

    .line 397
    :cond_0
    invoke-virtual {p0}, Landroid/graphics/Bitmap;->getWidth()I

    move-result v1

    invoke-virtual {p0}, Landroid/graphics/Bitmap;->getHeight()I

    move-result v2

    invoke-static {v1, v2, v0}, Lorg/cocos2dx/lib/Cocos2dxBitmap;->nativeInitBitmapDC(II[B)V

    goto :goto_0
.end method

.method private static native nativeInitBitmapDC(II[B)V
.end method

.method private static newPaint(Ljava/lang/String;II)Landroid/graphics/Paint;
    .locals 8
    .parameter "fontName"
    .parameter "fontSize"
    .parameter "alignment"

    .prologue
    const/4 v7, 0x0

    .line 308
    new-instance v2, Landroid/graphics/Paint;

    invoke-direct {v2}, Landroid/graphics/Paint;-><init>()V

    .line 309
    .local v2, paint:Landroid/graphics/Paint;
    const/4 v4, -0x1

    invoke-virtual {v2, v4}, Landroid/graphics/Paint;->setColor(I)V

    .line 310
    int-to-float v4, p1

    invoke-virtual {v2, v4}, Landroid/graphics/Paint;->setTextSize(F)V

    .line 311
    const/4 v4, 0x1

    invoke-virtual {v2, v4}, Landroid/graphics/Paint;->setAntiAlias(Z)V

    .line 316
    const-string v4, ".ttf"

    invoke-virtual {p0, v4}, Ljava/lang/String;->endsWith(Ljava/lang/String;)Z

    move-result v4

    if-eqz v4, :cond_0

    .line 319
    :try_start_0
    sget-object v4, Lorg/cocos2dx/lib/Cocos2dxBitmap;->context:Landroid/content/Context;

    invoke-static {v4, p0}, Lorg/cocos2dx/lib/Cocos2dxTypefaces;->get(Landroid/content/Context;Ljava/lang/String;)Landroid/graphics/Typeface;

    move-result-object v3

    .line 320
    .local v3, typeFace:Landroid/graphics/Typeface;
    invoke-virtual {v2, v3}, Landroid/graphics/Paint;->setTypeface(Landroid/graphics/Typeface;)Landroid/graphics/Typeface;
    :try_end_0
    .catch Ljava/lang/Exception; {:try_start_0 .. :try_end_0} :catch_0

    .line 335
    .end local v3           #typeFace:Landroid/graphics/Typeface;
    :goto_0
    and-int/lit8 v1, p2, 0xf

    .line 336
    .local v1, hAlignment:I
    packed-switch v1, :pswitch_data_0

    .line 350
    sget-object v4, Landroid/graphics/Paint$Align;->LEFT:Landroid/graphics/Paint$Align;

    invoke-virtual {v2, v4}, Landroid/graphics/Paint;->setTextAlign(Landroid/graphics/Paint$Align;)V

    .line 354
    :goto_1
    return-object v2

    .line 321
    .end local v1           #hAlignment:I
    :catch_0
    move-exception v0

    .line 322
    .local v0, e:Ljava/lang/Exception;
    const-string v4, "Cocos2dxBitmap"

    .line 323
    new-instance v5, Ljava/lang/StringBuilder;

    const-string v6, "error to create ttf type face: "

    invoke-direct {v5, v6}, Ljava/lang/StringBuilder;-><init>(Ljava/lang/String;)V

    invoke-virtual {v5, p0}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    move-result-object v5

    invoke-virtual {v5}, Ljava/lang/StringBuilder;->toString()Ljava/lang/String;

    move-result-object v5

    .line 322
    invoke-static {v4, v5}, Landroid/util/Log;->e(Ljava/lang/String;Ljava/lang/String;)I

    .line 328
    invoke-static {p0, v7}, Landroid/graphics/Typeface;->create(Ljava/lang/String;I)Landroid/graphics/Typeface;

    move-result-object v4

    invoke-virtual {v2, v4}, Landroid/graphics/Paint;->setTypeface(Landroid/graphics/Typeface;)Landroid/graphics/Typeface;

    goto :goto_0

    .line 332
    .end local v0           #e:Ljava/lang/Exception;
    :cond_0
    invoke-static {p0, v7}, Landroid/graphics/Typeface;->create(Ljava/lang/String;I)Landroid/graphics/Typeface;

    move-result-object v4

    invoke-virtual {v2, v4}, Landroid/graphics/Paint;->setTypeface(Landroid/graphics/Typeface;)Landroid/graphics/Typeface;

    goto :goto_0

    .line 338
    .restart local v1       #hAlignment:I
    :pswitch_0
    sget-object v4, Landroid/graphics/Paint$Align;->CENTER:Landroid/graphics/Paint$Align;

    invoke-virtual {v2, v4}, Landroid/graphics/Paint;->setTextAlign(Landroid/graphics/Paint$Align;)V

    goto :goto_1

    .line 342
    :pswitch_1
    sget-object v4, Landroid/graphics/Paint$Align;->LEFT:Landroid/graphics/Paint$Align;

    invoke-virtual {v2, v4}, Landroid/graphics/Paint;->setTextAlign(Landroid/graphics/Paint$Align;)V

    goto :goto_1

    .line 346
    :pswitch_2
    sget-object v4, Landroid/graphics/Paint$Align;->RIGHT:Landroid/graphics/Paint$Align;

    invoke-virtual {v2, v4}, Landroid/graphics/Paint;->setTextAlign(Landroid/graphics/Paint$Align;)V

    goto :goto_1

    .line 336
    nop

    :pswitch_data_0
    .packed-switch 0x1
        :pswitch_1
        :pswitch_2
        :pswitch_0
    .end packed-switch
.end method

.method private static refactorString(Ljava/lang/String;)Ljava/lang/String;
    .locals 5
    .parameter "str"

    .prologue
    .line 359
    const-string v3, ""

    invoke-virtual {p0, v3}, Ljava/lang/String;->compareTo(Ljava/lang/String;)I

    move-result v3

    if-nez v3, :cond_0

    .line 360
    const-string v3, " "

    .line 388
    :goto_0
    return-object v3

    .line 370
    :cond_0
    new-instance v2, Ljava/lang/StringBuilder;

    invoke-direct {v2, p0}, Ljava/lang/StringBuilder;-><init>(Ljava/lang/String;)V

    .line 371
    .local v2, strBuilder:Ljava/lang/StringBuilder;
    const/4 v1, 0x0

    .line 372
    .local v1, start:I
    const-string v3, "\n"

    invoke-virtual {v2, v3}, Ljava/lang/StringBuilder;->indexOf(Ljava/lang/String;)I

    move-result v0

    .line 373
    .local v0, index:I
    :goto_1
    const/4 v3, -0x1

    if-ne v0, v3, :cond_2

    .line 388
    :cond_1
    invoke-virtual {v2}, Ljava/lang/StringBuilder;->toString()Ljava/lang/String;

    move-result-object v3

    goto :goto_0

    .line 374
    :cond_2
    if-eqz v0, :cond_3

    add-int/lit8 v3, v0, -0x1

    invoke-virtual {v2, v3}, Ljava/lang/StringBuilder;->charAt(I)C

    move-result v3

    const/16 v4, 0xa

    if-ne v3, v4, :cond_4

    .line 375
    :cond_3
    const-string v3, " "

    invoke-virtual {v2, v1, v3}, Ljava/lang/StringBuilder;->insert(ILjava/lang/String;)Ljava/lang/StringBuilder;

    .line 376
    add-int/lit8 v1, v0, 0x2

    .line 381
    :goto_2
    invoke-virtual {v2}, Ljava/lang/StringBuilder;->length()I

    move-result v3

    if-gt v1, v3, :cond_1

    invoke-virtual {v2}, Ljava/lang/StringBuilder;->length()I

    move-result v3

    if-eq v0, v3, :cond_1

    .line 385
    const-string v3, "\n"

    invoke-virtual {v2, v3, v1}, Ljava/lang/StringBuilder;->indexOf(Ljava/lang/String;I)I

    move-result v0

    goto :goto_1

    .line 378
    :cond_4
    add-int/lit8 v1, v0, 0x1

    goto :goto_2
.end method

.method public static setContext(Landroid/content/Context;)V
    .locals 0
    .parameter "context"

    .prologue
    .line 55
    sput-object p0, Lorg/cocos2dx/lib/Cocos2dxBitmap;->context:Landroid/content/Context;

    .line 56
    return-void
.end method

.method private static splitString(Ljava/lang/String;IILandroid/graphics/Paint;)[Ljava/lang/String;
    .locals 15
    .parameter "content"
    .parameter "maxHeight"
    .parameter "maxWidth"
    .parameter "paint"

    .prologue
    .line 192
    const-string v11, "\\n"

    invoke-virtual {p0, v11}, Ljava/lang/String;->split(Ljava/lang/String;)[Ljava/lang/String;

    move-result-object v7

    .line 193
    .local v7, lines:[Ljava/lang/String;
    const/4 v9, 0x0

    .line 194
    .local v9, ret:[Ljava/lang/String;
    invoke-virtual/range {p3 .. p3}, Landroid/graphics/Paint;->getFontMetricsInt()Landroid/graphics/Paint$FontMetricsInt;

    move-result-object v2

    .line 195
    .local v2, fm:Landroid/graphics/Paint$FontMetricsInt;
    iget v11, v2, Landroid/graphics/Paint$FontMetricsInt;->bottom:I

    iget v12, v2, Landroid/graphics/Paint$FontMetricsInt;->top:I

    sub-int/2addr v11, v12

    int-to-double v11, v11

    invoke-static {v11, v12}, Ljava/lang/Math;->ceil(D)D

    move-result-wide v11

    double-to-int v3, v11

    .line 196
    .local v3, heightPerLine:I
    div-int v8, p1, v3

    .line 198
    .local v8, maxLines:I
    if-eqz p2, :cond_6

    .line 199
    new-instance v10, Ljava/util/LinkedList;

    invoke-direct {v10}, Ljava/util/LinkedList;-><init>()V

    .line 200
    .local v10, strList:Ljava/util/LinkedList;,"Ljava/util/LinkedList<Ljava/lang/String;>;"
    array-length v12, v7

    const/4 v11, 0x0

    :goto_0
    if-lt v11, v12, :cond_2

    .line 224
    :cond_0
    if-lez v8, :cond_1

    invoke-virtual {v10}, Ljava/util/LinkedList;->size()I

    move-result v11

    if-le v11, v8, :cond_1

    .line 225
    :goto_1
    invoke-virtual {v10}, Ljava/util/LinkedList;->size()I

    move-result v11

    if-gt v11, v8, :cond_5

    .line 230
    :cond_1
    invoke-virtual {v10}, Ljava/util/LinkedList;->size()I

    move-result v11

    new-array v9, v11, [Ljava/lang/String;

    .line 231
    invoke-virtual {v10, v9}, Ljava/util/LinkedList;->toArray([Ljava/lang/Object;)[Ljava/lang/Object;

    .line 248
    .end local v10           #strList:Ljava/util/LinkedList;,"Ljava/util/LinkedList<Ljava/lang/String;>;"
    :goto_2
    return-object v9

    .line 200
    .restart local v10       #strList:Ljava/util/LinkedList;,"Ljava/util/LinkedList<Ljava/lang/String;>;"
    :cond_2
    aget-object v5, v7, v11

    .line 205
    .local v5, line:Ljava/lang/String;
    move-object/from16 v0, p3

    invoke-virtual {v0, v5}, Landroid/graphics/Paint;->measureText(Ljava/lang/String;)F

    move-result v13

    float-to-double v13, v13

    invoke-static {v13, v14}, Ljava/lang/Math;->ceil(D)D

    move-result-wide v13

    double-to-int v6, v13

    .line 206
    .local v6, lineWidth:I
    move/from16 v0, p2

    if-le v6, v0, :cond_4

    .line 207
    move-object/from16 v0, p3

    move/from16 v1, p2

    invoke-static {v0, v5, v1}, Lorg/cocos2dx/lib/Cocos2dxBitmap;->divideStringWithMaxWidth(Landroid/graphics/Paint;Ljava/lang/String;I)Ljava/util/LinkedList;

    move-result-object v13

    invoke-virtual {v10, v13}, Ljava/util/LinkedList;->addAll(Ljava/util/Collection;)Z

    .line 216
    :goto_3
    if-lez v8, :cond_3

    invoke-virtual {v10}, Ljava/util/LinkedList;->size()I

    move-result v13

    if-ge v13, v8, :cond_0

    .line 200
    :cond_3
    add-int/lit8 v11, v11, 0x1

    goto :goto_0

    .line 210
    :cond_4
    invoke-virtual {v10, v5}, Ljava/util/LinkedList;->add(Ljava/lang/Object;)Z

    goto :goto_3

    .line 226
    .end local v5           #line:Ljava/lang/String;
    .end local v6           #lineWidth:I
    :cond_5
    invoke-virtual {v10}, Ljava/util/LinkedList;->removeLast()Ljava/lang/Object;

    goto :goto_1

    .line 233
    .end local v10           #strList:Ljava/util/LinkedList;,"Ljava/util/LinkedList<Ljava/lang/String;>;"
    :cond_6
    if-eqz p1, :cond_8

    array-length v11, v7

    if-le v11, v8, :cond_8

    .line 237
    new-instance v10, Ljava/util/LinkedList;

    invoke-direct {v10}, Ljava/util/LinkedList;-><init>()V

    .line 238
    .restart local v10       #strList:Ljava/util/LinkedList;,"Ljava/util/LinkedList<Ljava/lang/String;>;"
    const/4 v4, 0x0

    .local v4, i:I
    :goto_4
    if-lt v4, v8, :cond_7

    .line 241
    invoke-virtual {v10}, Ljava/util/LinkedList;->size()I

    move-result v11

    new-array v9, v11, [Ljava/lang/String;

    .line 242
    invoke-virtual {v10, v9}, Ljava/util/LinkedList;->toArray([Ljava/lang/Object;)[Ljava/lang/Object;

    goto :goto_2

    .line 239
    :cond_7
    aget-object v11, v7, v4

    invoke-virtual {v10, v11}, Ljava/util/LinkedList;->add(Ljava/lang/Object;)Z

    .line 238
    add-int/lit8 v4, v4, 0x1

    goto :goto_4

    .line 245
    .end local v4           #i:I
    .end local v10           #strList:Ljava/util/LinkedList;,"Ljava/util/LinkedList<Ljava/lang/String;>;"
    :cond_8
    move-object v9, v7

    goto :goto_2
.end method
