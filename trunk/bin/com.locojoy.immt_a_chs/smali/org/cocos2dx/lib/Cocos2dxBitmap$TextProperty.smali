.class Lorg/cocos2dx/lib/Cocos2dxBitmap$TextProperty;
.super Ljava/lang/Object;
.source "Cocos2dxBitmap.java"


# annotations
.annotation system Ldalvik/annotation/EnclosingClass;
    value = Lorg/cocos2dx/lib/Cocos2dxBitmap;
.end annotation

.annotation system Ldalvik/annotation/InnerClass;
    accessFlags = 0xa
    name = "TextProperty"
.end annotation


# instance fields
.field heightPerLine:I

.field lines:[Ljava/lang/String;

.field maxWidth:I

.field totalHeight:I


# direct methods
.method constructor <init>(II[Ljava/lang/String;)V
    .locals 1
    .parameter "w"
    .parameter "h"
    .parameter "lines"

    .prologue
    .line 151
    invoke-direct {p0}, Ljava/lang/Object;-><init>()V

    .line 152
    iput p1, p0, Lorg/cocos2dx/lib/Cocos2dxBitmap$TextProperty;->maxWidth:I

    .line 153
    iput p2, p0, Lorg/cocos2dx/lib/Cocos2dxBitmap$TextProperty;->heightPerLine:I

    .line 154
    array-length v0, p3

    mul-int/2addr v0, p2

    iput v0, p0, Lorg/cocos2dx/lib/Cocos2dxBitmap$TextProperty;->totalHeight:I

    .line 155
    iput-object p3, p0, Lorg/cocos2dx/lib/Cocos2dxBitmap$TextProperty;->lines:[Ljava/lang/String;

    .line 156
    return-void
.end method
