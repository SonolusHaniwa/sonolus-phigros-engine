class Initialization: public Archetype {
    public:

    static constexpr const char* name = "Phigros Initialization";
    bool hasInput = false;

	int preprocessOrder = 2;
    SonolusApi preprocess() {
    	FUNCBEGIN
        // 界面开关参数
        IF (!sonolusCombo && HasSkinSprite(Sprites.PauseButton)) {
            let menuL = stage.l + 0.005 * stage.w;
            let menuR = stage.l + 0.05 * stage.w;
            let menuT = stage.t + 1.0 / 1080.0 * stage.h;
            let menuB = stage.t - 0.095 * stage.h;
            let menuX = (menuR + menuL) / 2;
            let menuY = (menuT + menuB) / 2;
            let menuWidth = menuR - menuL;
            let menuHeight = menuT - menuB;
            ui.menu.set(menuX, menuY, 0.5, 0.5, menuWidth, menuHeight, 0, 0, HorizontalAlign.Center, true);
        } ELSE {
            let menuWidth = 0.15 * ui.menuConfiguration.scale;
            let menuHeight = 0.15 * ui.menuConfiguration.scale;
            let menuX = stage.l + interfaceGap;
            let menuY = stage.t - interfaceGap;
            ui.menu.set(menuX, menuY, 0, 1, menuWidth, menuHeight, 0, ui.menuConfiguration.alpha, HorizontalAlign.Center, true);
        } FI
        
        // 首要指标参数
        IF (sonolusPrimaryMetric || !HasSkinSprite(Sprites.Combo0)) {
            let primaryMetricBarWidth = 0.75 * ui.primaryMetricConfiguration.scale;
            let primaryMetricBarHeight = 0.15 * ui.primaryMetricConfiguration.scale;
            let primaryMetricBarX = stage.r - interfaceGap;
            let primaryMetricBarY = stage.t - interfaceGap;
            let primaryMetricValueWidth = 0 * ui.primaryMetricConfiguration.scale;
            let primaryMetricValueHeight = 0.08 * ui.primaryMetricConfiguration.scale;
            let primaryMetricValueX = primaryMetricBarX - 0.035 * ui.primaryMetricConfiguration.scale;
            let primaryMetricValueY = primaryMetricBarY - 0.035 * ui.primaryMetricConfiguration.scale;
            ui.primaryMetricBar.set(primaryMetricBarX, primaryMetricBarY, 1, 1, primaryMetricBarWidth, primaryMetricBarHeight, 0, ui.primaryMetricConfiguration.alpha, HorizontalAlign.Left, true);
            ui.primaryMetricValue.set(primaryMetricValueX, primaryMetricValueY, 1, 1, primaryMetricValueWidth, primaryMetricValueHeight, 0, ui.primaryMetricConfiguration.alpha, HorizontalAlign.Right, false);
        } FI
        
        // 连击指标参数
        IF (sonolusCombo || !HasSkinSprite(Sprites.ComboText) || !HasSkinSprite(Sprites.Combo0)) {
            let comboValueWidth = 0 * ui.comboConfiguration.scale;
            let comboValueHeight = stage.h * 0.048 * ui.comboConfiguration.scale;
            let comboValueX = 0;
            let comboValueY = stage.t - stage.h * 0.054 * ui.comboConfiguration.scale;
            let comboTextWidth = 0 * ui.comboConfiguration.scale;
            let comboTextHeight = stage.h * 0.017 * ui.comboConfiguration.scale;
            let comboTextX = 0;
            let comboTextY = stage.t - stage.h * 0.0985 * ui.comboConfiguration.scale;
            ui.comboValue.set(comboValueX, comboValueY, 0.5, 0.5, comboValueWidth, comboValueHeight, 0, ui.comboConfiguration.alpha, HorizontalAlign.Center, false);
            ui.comboText.set(comboTextX, comboTextY, 0.5, 0.5, comboTextWidth, comboTextHeight, 0, ui.comboConfiguration.alpha, HorizontalAlign.Center, false);         
        } FI
        
        // 进度条参数
        let progressWidth = stage.w - interfaceGap * 2;
        let progressHeight = 0.15 * ui.progressConfiguration.scale;
        let progressX = stage.l + interfaceGap;
        let progressY = stage.b + interfaceGap;
        ui.progress.set(progressX, progressY, 0, 0, progressWidth, progressHeight, 0, ui.progressConfiguration.alpha, HorizontalAlign.Center, true);
        
        LevelScore.set(0, score.perfect);
        LevelScore.set(1, score.great);
        LevelScore.set(2, score.good);
        buckets[Buckets.note].set(-1 * judgment.perfect, judgment.perfect, -1 * judgment.great, judgment.great, -1 * judgment.good, judgment.good);
        buckets[Buckets.drag].set(-1 * judgment.good, judgment.good, -1 * judgment.good, judgment.good, -1 * judgment.good, judgment.good);
        buckets[Buckets.hold].set(-1 * judgment.perfect, judgment.perfect, -1 * judgment.good, judgment.good, -1 * judgment.good, judgment.good);
        buckets[Buckets.flick].set(-1 * judgment.good, judgment.good, -1 * judgment.good, judgment.good, -1 * judgment.good, judgment.good);

        combo = 0;
        judgeStatus = If(hasIndicator, 2, 0);
        maxCombo = 0;
        notes = 0;
        accscore = 0;
        return VOID;
    }

	SonolusApi spawnTime() { return 0; }
	SonolusApi despawnTime() { return 0; }
};