.class public Lorg/cocos2dx/lib/Cocos2dxActivity;
.super Landroid/app/Activity;
.source "Cocos2dxActivity.java"


# static fields
.field private static final HANDLER_SHOW_DIALOG:I = 0x1

.field private static accelerometer:Lorg/cocos2dx/lib/Cocos2dxAccelerometer;

.field private static accelerometerEnabled:Z

.field private static backgroundMusicPlayer:Lorg/cocos2dx/lib/Cocos2dxMusic;

.field private static handler:Landroid/os/Handler;

.field private static packageName:Ljava/lang/String;

.field private static soundPlayer:Lorg/cocos2dx/lib/Cocos2dxSound;


# direct methods
.method static constructor <clinit>()V
    .locals 1

    .prologue
    .line 45
    const/4 v0, 0x0

    sput-boolean v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->accelerometerEnabled:Z

    .line 48
    return-void
.end method

.method public constructor <init>()V
    .locals 0

    .prologue
    .line 41
    invoke-direct {p0}, Landroid/app/Activity;-><init>()V

    return-void
.end method

.method static synthetic access$0(Lorg/cocos2dx/lib/Cocos2dxActivity;Ljava/lang/String;Ljava/lang/String;)V
    .locals 0
    .parameter
    .parameter
    .parameter

    .prologue
    .line 230
    invoke-direct {p0, p1, p2}, Lorg/cocos2dx/lib/Cocos2dxActivity;->showDialog(Ljava/lang/String;Ljava/lang/String;)V

    return-void
.end method

.method public static disableAccelerometer()V
    .locals 1

    .prologue
    .line 98
    const/4 v0, 0x0

    sput-boolean v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->accelerometerEnabled:Z

    .line 99
    sget-object v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->accelerometer:Lorg/cocos2dx/lib/Cocos2dxAccelerometer;

    invoke-virtual {v0}, Lorg/cocos2dx/lib/Cocos2dxAccelerometer;->disable()V

    .line 100
    return-void
.end method

.method public static enableAccelerometer()V
    .locals 1

    .prologue
    .line 93
    const/4 v0, 0x1

    sput-boolean v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->accelerometerEnabled:Z

    .line 94
    sget-object v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->accelerometer:Lorg/cocos2dx/lib/Cocos2dxAccelerometer;

    invoke-virtual {v0}, Lorg/cocos2dx/lib/Cocos2dxAccelerometer;->enable()V

    .line 95
    return-void
.end method

.method public static end()V
    .locals 1

    .prologue
    .line 183
    sget-object v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->backgroundMusicPlayer:Lorg/cocos2dx/lib/Cocos2dxMusic;

    invoke-virtual {v0}, Lorg/cocos2dx/lib/Cocos2dxMusic;->end()V

    .line 184
    sget-object v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->soundPlayer:Lorg/cocos2dx/lib/Cocos2dxSound;

    invoke-virtual {v0}, Lorg/cocos2dx/lib/Cocos2dxSound;->end()V

    .line 185
    return-void
.end method

.method public static getBackgroundMusicVolume()F
    .locals 1

    .prologue
    .line 131
    sget-object v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->backgroundMusicPlayer:Lorg/cocos2dx/lib/Cocos2dxMusic;

    invoke-virtual {v0}, Lorg/cocos2dx/lib/Cocos2dxMusic;->getBackgroundVolume()F

    move-result v0

    return v0
.end method

.method public static getCocos2dxPackageName()Ljava/lang/String;
    .locals 1

    .prologue
    .line 188
    sget-object v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->packageName:Ljava/lang/String;

    return-object v0
.end method

.method public static getCurrentLanguage()Ljava/lang/String;
    .locals 2

    .prologue
    .line 80
    invoke-static {}, Ljava/util/Locale;->getDefault()Ljava/util/Locale;

    move-result-object v1

    invoke-virtual {v1}, Ljava/util/Locale;->getLanguage()Ljava/lang/String;

    move-result-object v0

    .line 81
    .local v0, languageName:Ljava/lang/String;
    return-object v0
.end method

.method public static getEffectsVolume()F
    .locals 1

    .prologue
    .line 155
    sget-object v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->soundPlayer:Lorg/cocos2dx/lib/Cocos2dxSound;

    invoke-virtual {v0}, Lorg/cocos2dx/lib/Cocos2dxSound;->getEffectsVolume()F

    move-result v0

    return v0
.end method

.method public static isBackgroundMusicPlaying()Z
    .locals 1

    .prologue
    .line 127
    sget-object v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->backgroundMusicPlayer:Lorg/cocos2dx/lib/Cocos2dxMusic;

    invoke-virtual {v0}, Lorg/cocos2dx/lib/Cocos2dxMusic;->isBackgroundMusicPlaying()Z

    move-result v0

    return v0
.end method

.method private static native nativeSetPaths(Ljava/lang/String;)V
.end method

.method public static pauseAllEffects()V
    .locals 1

    .prologue
    .line 175
    sget-object v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->soundPlayer:Lorg/cocos2dx/lib/Cocos2dxSound;

    invoke-virtual {v0}, Lorg/cocos2dx/lib/Cocos2dxSound;->pauseAllEffects()V

    .line 176
    return-void
.end method

.method public static pauseBackgroundMusic()V
    .locals 1

    .prologue
    .line 115
    sget-object v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->backgroundMusicPlayer:Lorg/cocos2dx/lib/Cocos2dxMusic;

    invoke-virtual {v0}, Lorg/cocos2dx/lib/Cocos2dxMusic;->pauseBackgroundMusic()V

    .line 116
    return-void
.end method

.method public static pauseEffect(I)V
    .locals 1
    .parameter "soundId"

    .prologue
    .line 147
    sget-object v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->soundPlayer:Lorg/cocos2dx/lib/Cocos2dxSound;

    invoke-virtual {v0, p0}, Lorg/cocos2dx/lib/Cocos2dxSound;->pauseEffect(I)V

    .line 148
    return-void
.end method

.method public static playBackgroundMusic(Ljava/lang/String;Z)V
    .locals 1
    .parameter "path"
    .parameter "isLoop"

    .prologue
    .line 107
    sget-object v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->backgroundMusicPlayer:Lorg/cocos2dx/lib/Cocos2dxMusic;

    invoke-virtual {v0, p0, p1}, Lorg/cocos2dx/lib/Cocos2dxMusic;->playBackgroundMusic(Ljava/lang/String;Z)V

    .line 108
    return-void
.end method

.method public static playEffect(Ljava/lang/String;Z)I
    .locals 1
    .parameter "path"
    .parameter "isLoop"

    .prologue
    .line 139
    sget-object v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->soundPlayer:Lorg/cocos2dx/lib/Cocos2dxSound;

    invoke-virtual {v0, p0, p1}, Lorg/cocos2dx/lib/Cocos2dxSound;->playEffect(Ljava/lang/String;Z)I

    move-result v0

    return v0
.end method

.method public static preloadBackgroundMusic(Ljava/lang/String;)V
    .locals 1
    .parameter "path"

    .prologue
    .line 103
    sget-object v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->backgroundMusicPlayer:Lorg/cocos2dx/lib/Cocos2dxMusic;

    invoke-virtual {v0, p0}, Lorg/cocos2dx/lib/Cocos2dxMusic;->preloadBackgroundMusic(Ljava/lang/String;)V

    .line 104
    return-void
.end method

.method public static preloadEffect(Ljava/lang/String;)V
    .locals 1
    .parameter "path"

    .prologue
    .line 163
    sget-object v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->soundPlayer:Lorg/cocos2dx/lib/Cocos2dxSound;

    invoke-virtual {v0, p0}, Lorg/cocos2dx/lib/Cocos2dxSound;->preloadEffect(Ljava/lang/String;)I

    .line 164
    return-void
.end method

.method public static resumeAllEffects()V
    .locals 1

    .prologue
    .line 179
    sget-object v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->soundPlayer:Lorg/cocos2dx/lib/Cocos2dxSound;

    invoke-virtual {v0}, Lorg/cocos2dx/lib/Cocos2dxSound;->resumeAllEffects()V

    .line 180
    return-void
.end method

.method public static resumeBackgroundMusic()V
    .locals 1

    .prologue
    .line 119
    sget-object v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->backgroundMusicPlayer:Lorg/cocos2dx/lib/Cocos2dxMusic;

    invoke-virtual {v0}, Lorg/cocos2dx/lib/Cocos2dxMusic;->resumeBackgroundMusic()V

    .line 120
    return-void
.end method

.method public static resumeEffect(I)V
    .locals 1
    .parameter "soundId"

    .prologue
    .line 151
    sget-object v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->soundPlayer:Lorg/cocos2dx/lib/Cocos2dxSound;

    invoke-virtual {v0, p0}, Lorg/cocos2dx/lib/Cocos2dxSound;->resumeEffect(I)V

    .line 152
    return-void
.end method

.method public static rewindBackgroundMusic()V
    .locals 1

    .prologue
    .line 123
    sget-object v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->backgroundMusicPlayer:Lorg/cocos2dx/lib/Cocos2dxMusic;

    invoke-virtual {v0}, Lorg/cocos2dx/lib/Cocos2dxMusic;->rewindBackgroundMusic()V

    .line 124
    return-void
.end method

.method public static setBackgroundMusicVolume(F)V
    .locals 1
    .parameter "volume"

    .prologue
    .line 135
    sget-object v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->backgroundMusicPlayer:Lorg/cocos2dx/lib/Cocos2dxMusic;

    invoke-virtual {v0, p0}, Lorg/cocos2dx/lib/Cocos2dxMusic;->setBackgroundVolume(F)V

    .line 136
    return-void
.end method

.method public static setEffectsVolume(F)V
    .locals 1
    .parameter "volume"

    .prologue
    .line 159
    sget-object v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->soundPlayer:Lorg/cocos2dx/lib/Cocos2dxSound;

    invoke-virtual {v0, p0}, Lorg/cocos2dx/lib/Cocos2dxSound;->setEffectsVolume(F)V

    .line 160
    return-void
.end method

.method private showDialog(Ljava/lang/String;Ljava/lang/String;)V
    .locals 9
    .parameter "title"
    .parameter "message"

    .prologue
    .line 231
    new-instance v6, Landroid/app/AlertDialog$Builder;

    invoke-direct {v6, p0}, Landroid/app/AlertDialog$Builder;-><init>(Landroid/content/Context;)V

    .line 232
    invoke-virtual {v6, p1}, Landroid/app/AlertDialog$Builder;->setTitle(Ljava/lang/CharSequence;)Landroid/app/AlertDialog$Builder;

    move-result-object v6

    .line 233
    invoke-virtual {v6, p2}, Landroid/app/AlertDialog$Builder;->setMessage(Ljava/lang/CharSequence;)Landroid/app/AlertDialog$Builder;

    move-result-object v6

    .line 234
    const-string v7, "Ok"

    .line 235
    new-instance v8, Lorg/cocos2dx/lib/Cocos2dxActivity$2;

    invoke-direct {v8, p0}, Lorg/cocos2dx/lib/Cocos2dxActivity$2;-><init>(Lorg/cocos2dx/lib/Cocos2dxActivity;)V

    .line 234
    invoke-virtual {v6, v7, v8}, Landroid/app/AlertDialog$Builder;->setPositiveButton(Ljava/lang/CharSequence;Landroid/content/DialogInterface$OnClickListener;)Landroid/app/AlertDialog$Builder;

    move-result-object v6

    .line 240
    invoke-virtual {v6}, Landroid/app/AlertDialog$Builder;->create()Landroid/app/AlertDialog;

    move-result-object v0

    .line 241
    .local v0, dialog:Landroid/app/Dialog;
    invoke-virtual {v0}, Landroid/app/Dialog;->show()V

    .line 242
    new-instance v3, Landroid/util/DisplayMetrics;

    invoke-direct {v3}, Landroid/util/DisplayMetrics;-><init>()V

    .line 243
    .local v3, metrics:Landroid/util/DisplayMetrics;
    invoke-virtual {p0}, Lorg/cocos2dx/lib/Cocos2dxActivity;->getWindowManager()Landroid/view/WindowManager;

    move-result-object v6

    invoke-interface {v6}, Landroid/view/WindowManager;->getDefaultDisplay()Landroid/view/Display;

    move-result-object v6

    invoke-virtual {v6, v3}, Landroid/view/Display;->getMetrics(Landroid/util/DisplayMetrics;)V

    .line 244
    iget v5, v3, Landroid/util/DisplayMetrics;->widthPixels:I

    .line 245
    .local v5, width:I
    iget v2, v3, Landroid/util/DisplayMetrics;->heightPixels:I

    .line 246
    .local v2, height:I
    mul-int/lit8 v6, v5, 0x4

    int-to-float v6, v6

    const/high16 v7, 0x40a0

    div-float v4, v6, v7

    .line 247
    .local v4, w:F
    mul-int/lit8 v6, v2, 0x2

    int-to-float v6, v6

    const/high16 v7, 0x4040

    div-float v1, v6, v7

    .line 248
    .local v1, h:F
    invoke-virtual {v0}, Landroid/app/Dialog;->getWindow()Landroid/view/Window;

    move-result-object v6

    float-to-int v7, v4

    float-to-int v8, v1

    invoke-virtual {v6, v7, v8}, Landroid/view/Window;->setLayout(II)V

    .line 249
    return-void
.end method

.method public static showMessageBox(Ljava/lang/String;Ljava/lang/String;)V
    .locals 2
    .parameter "title"
    .parameter "message"

    .prologue
    .line 85
    new-instance v0, Landroid/os/Message;

    invoke-direct {v0}, Landroid/os/Message;-><init>()V

    .line 86
    .local v0, msg:Landroid/os/Message;
    const/4 v1, 0x1

    iput v1, v0, Landroid/os/Message;->what:I

    .line 87
    new-instance v1, Lorg/cocos2dx/lib/DialogMessage;

    invoke-direct {v1, p0, p1}, Lorg/cocos2dx/lib/DialogMessage;-><init>(Ljava/lang/String;Ljava/lang/String;)V

    iput-object v1, v0, Landroid/os/Message;->obj:Ljava/lang/Object;

    .line 89
    sget-object v1, Lorg/cocos2dx/lib/Cocos2dxActivity;->handler:Landroid/os/Handler;

    invoke-virtual {v1, v0}, Landroid/os/Handler;->sendMessage(Landroid/os/Message;)Z

    .line 90
    return-void
.end method

.method public static stopAllEffects()V
    .locals 1

    .prologue
    .line 171
    sget-object v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->soundPlayer:Lorg/cocos2dx/lib/Cocos2dxSound;

    invoke-virtual {v0}, Lorg/cocos2dx/lib/Cocos2dxSound;->stopAllEffects()V

    .line 172
    return-void
.end method

.method public static stopBackgroundMusic()V
    .locals 1

    .prologue
    .line 111
    sget-object v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->backgroundMusicPlayer:Lorg/cocos2dx/lib/Cocos2dxMusic;

    invoke-virtual {v0}, Lorg/cocos2dx/lib/Cocos2dxMusic;->stopBackgroundMusic()V

    .line 112
    return-void
.end method

.method public static stopEffect(I)V
    .locals 1
    .parameter "soundId"

    .prologue
    .line 143
    sget-object v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->soundPlayer:Lorg/cocos2dx/lib/Cocos2dxSound;

    invoke-virtual {v0, p0}, Lorg/cocos2dx/lib/Cocos2dxSound;->stopEffect(I)V

    .line 144
    return-void
.end method

.method public static terminateProcess()V
    .locals 1

    .prologue
    .line 192
    invoke-static {}, Landroid/os/Process;->myPid()I

    move-result v0

    invoke-static {v0}, Landroid/os/Process;->killProcess(I)V

    .line 193
    return-void
.end method

.method public static unloadEffect(Ljava/lang/String;)V
    .locals 1
    .parameter "path"

    .prologue
    .line 167
    sget-object v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->soundPlayer:Lorg/cocos2dx/lib/Cocos2dxSound;

    invoke-virtual {v0, p0}, Lorg/cocos2dx/lib/Cocos2dxSound;->unloadEffect(Ljava/lang/String;)V

    .line 168
    return-void
.end method


# virtual methods
.method protected onCreate(Landroid/os/Bundle;)V
    .locals 2
    .parameter "savedInstanceState"

    .prologue
    .line 54
    invoke-super {p0, p1}, Landroid/app/Activity;->onCreate(Landroid/os/Bundle;)V

    .line 57
    new-instance v0, Landroid/util/DisplayMetrics;

    invoke-direct {v0}, Landroid/util/DisplayMetrics;-><init>()V

    .line 58
    .local v0, dm:Landroid/util/DisplayMetrics;
    invoke-virtual {p0}, Lorg/cocos2dx/lib/Cocos2dxActivity;->getWindowManager()Landroid/view/WindowManager;

    move-result-object v1

    invoke-interface {v1}, Landroid/view/WindowManager;->getDefaultDisplay()Landroid/view/Display;

    move-result-object v1

    invoke-virtual {v1, v0}, Landroid/view/Display;->getMetrics(Landroid/util/DisplayMetrics;)V

    .line 59
    new-instance v1, Lorg/cocos2dx/lib/Cocos2dxAccelerometer;

    invoke-direct {v1, p0}, Lorg/cocos2dx/lib/Cocos2dxAccelerometer;-><init>(Landroid/content/Context;)V

    sput-object v1, Lorg/cocos2dx/lib/Cocos2dxActivity;->accelerometer:Lorg/cocos2dx/lib/Cocos2dxAccelerometer;

    .line 62
    new-instance v1, Lorg/cocos2dx/lib/Cocos2dxMusic;

    invoke-direct {v1, p0}, Lorg/cocos2dx/lib/Cocos2dxMusic;-><init>(Landroid/content/Context;)V

    sput-object v1, Lorg/cocos2dx/lib/Cocos2dxActivity;->backgroundMusicPlayer:Lorg/cocos2dx/lib/Cocos2dxMusic;

    .line 63
    new-instance v1, Lorg/cocos2dx/lib/Cocos2dxSound;

    invoke-direct {v1, p0}, Lorg/cocos2dx/lib/Cocos2dxSound;-><init>(Landroid/content/Context;)V

    sput-object v1, Lorg/cocos2dx/lib/Cocos2dxActivity;->soundPlayer:Lorg/cocos2dx/lib/Cocos2dxSound;

    .line 66
    invoke-static {p0}, Lorg/cocos2dx/lib/Cocos2dxBitmap;->setContext(Landroid/content/Context;)V

    .line 68
    new-instance v1, Lorg/cocos2dx/lib/Cocos2dxActivity$1;

    invoke-direct {v1, p0}, Lorg/cocos2dx/lib/Cocos2dxActivity$1;-><init>(Lorg/cocos2dx/lib/Cocos2dxActivity;)V

    sput-object v1, Lorg/cocos2dx/lib/Cocos2dxActivity;->handler:Landroid/os/Handler;

    .line 77
    return-void
.end method

.method protected onPause()V
    .locals 1

    .prologue
    .line 205
    invoke-super {p0}, Landroid/app/Activity;->onPause()V

    .line 206
    sget-boolean v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->accelerometerEnabled:Z

    if-eqz v0, :cond_0

    .line 207
    sget-object v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->accelerometer:Lorg/cocos2dx/lib/Cocos2dxAccelerometer;

    invoke-virtual {v0}, Lorg/cocos2dx/lib/Cocos2dxAccelerometer;->disable()V

    .line 209
    :cond_0
    return-void
.end method

.method protected onResume()V
    .locals 1

    .prologue
    .line 197
    invoke-super {p0}, Landroid/app/Activity;->onResume()V

    .line 198
    sget-boolean v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->accelerometerEnabled:Z

    if-eqz v0, :cond_0

    .line 199
    sget-object v0, Lorg/cocos2dx/lib/Cocos2dxActivity;->accelerometer:Lorg/cocos2dx/lib/Cocos2dxAccelerometer;

    invoke-virtual {v0}, Lorg/cocos2dx/lib/Cocos2dxAccelerometer;->enable()V

    .line 201
    :cond_0
    return-void
.end method

.method protected setPackageName(Ljava/lang/String;)V
    .locals 6
    .parameter "packageName"

    .prologue
    .line 212
    sput-object p1, Lorg/cocos2dx/lib/Cocos2dxActivity;->packageName:Ljava/lang/String;

    .line 214
    const-string v0, ""

    .line 215
    .local v0, apkFilePath:Ljava/lang/String;
    const/4 v1, 0x0

    .line 216
    .local v1, appInfo:Landroid/content/pm/ApplicationInfo;
    invoke-virtual {p0}, Lorg/cocos2dx/lib/Cocos2dxActivity;->getApplication()Landroid/app/Application;

    move-result-object v4

    invoke-virtual {v4}, Landroid/app/Application;->getPackageManager()Landroid/content/pm/PackageManager;

    move-result-object v3

    .line 218
    .local v3, packMgmr:Landroid/content/pm/PackageManager;
    const/4 v4, 0x0

    :try_start_0
    invoke-virtual {v3, p1, v4}, Landroid/content/pm/PackageManager;->getApplicationInfo(Ljava/lang/String;I)Landroid/content/pm/ApplicationInfo;
    :try_end_0
    .catch Landroid/content/pm/PackageManager$NameNotFoundException; {:try_start_0 .. :try_end_0} :catch_0

    move-result-object v1

    .line 223
    iget-object v0, v1, Landroid/content/pm/ApplicationInfo;->sourceDir:Ljava/lang/String;

    .line 224
    const-string v4, "apk path"

    invoke-static {v4, v0}, Landroid/util/Log;->w(Ljava/lang/String;Ljava/lang/String;)I

    .line 227
    invoke-static {v0}, Lorg/cocos2dx/lib/Cocos2dxActivity;->nativeSetPaths(Ljava/lang/String;)V

    .line 228
    return-void

    .line 219
    :catch_0
    move-exception v2

    .line 220
    .local v2, e:Landroid/content/pm/PackageManager$NameNotFoundException;
    invoke-virtual {v2}, Landroid/content/pm/PackageManager$NameNotFoundException;->printStackTrace()V

    .line 221
    new-instance v4, Ljava/lang/RuntimeException;

    const-string v5, "Unable to locate assets, aborting..."

    invoke-direct {v4, v5}, Ljava/lang/RuntimeException;-><init>(Ljava/lang/String;)V

    throw v4
.end method
