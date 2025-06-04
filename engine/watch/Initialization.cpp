class Initialization: public Archetype {
    public:

    string name = "Phigros Initialization";
    bool hasInput = false;

	int preprocessOrder = -114514;
    SonolusApi preprocess() {
        // 界面开关参数
        if (!sonolusPause && HasSkinSprite(Sprites.PauseButton)) {
            var menuL = stage.l + 0.005 * stage.w;
            var menuR = stage.l + 0.05 * stage.w;
            var menuT = stage.t + 1.0 / 1080.0 * stage.h;
            var menuB = stage.t - 0.095 * stage.h;
            var menuX = (menuR + menuL) / 2;
            var menuY = (menuT + menuB) / 2;
            var menuWidth = menuR - menuL;
            var menuHeight = menuT - menuB;
            setUI(
                ui.menu,
                menuX, menuY, 
                0.5, 0.5, 
                menuWidth, menuHeight, 
                0, 
                0, 
                HorizontalAlign.Center, 
                true
            );
        } else {
            var menuWidth = 0.15 * configuration.menu.scale;
            var menuHeight = 0.15 * configuration.menu.scale;
            var menuX = stage.l + interfaceGap;
            var menuY = stage.t - interfaceGap;
            setUI(
                ui.menu,
                menuX, menuY, 
                0, 1, 
                menuWidth, menuHeight, 
                0, 
                configuration.menu.alpha, 
                HorizontalAlign.Center, 
                true
            );
        }
        
        // 首要指标参数
        if (sonolusPrimaryMetric || !HasSkinSprite(Sprites.Combo0)) {
            var primaryMetricBarWidth = 0.75 * configuration.primaryMetric.scale;
            var primaryMetricBarHeight = 0.15 * configuration.primaryMetric.scale;
            var primaryMetricBarX = stage.r - interfaceGap;
            var primaryMetricBarY = stage.t - interfaceGap;
            var primaryMetricValueWidth = 0 * configuration.primaryMetric.scale;
            var primaryMetricValueHeight = 0.08 * configuration.primaryMetric.scale;
            var primaryMetricValueX = primaryMetricBarX - 0.035 * configuration.primaryMetric.scale;
            var primaryMetricValueY = primaryMetricBarY - 0.035 * configuration.primaryMetric.scale;
            setUI(
                ui.primaryMetricBar,
                primaryMetricBarX, primaryMetricBarY, 
                1, 1, 
                primaryMetricBarWidth, primaryMetricBarHeight, 
                0, 
                configuration.primaryMetric.alpha, 
                HorizontalAlign.Left, 
                true
            );
            setUI(
                ui.primaryMetricValue,
                primaryMetricValueX, primaryMetricValueY, 
                1, 1, 
                primaryMetricValueWidth, primaryMetricValueHeight, 
                0, 
                configuration.primaryMetric.alpha, 
                HorizontalAlign.Right, 
                false
            );
        }
        
        // 连击指标参数
        if (sonolusCombo || !HasSkinSprite(Sprites.ComboText) || !HasSkinSprite(Sprites.Combo0)) {
            var comboValueWidth = 0 * configuration.combo.scale;
            var comboValueHeight = stage.h * 0.048 * configuration.combo.scale;
            var comboValueX = 0;
            var comboValueY = stage.t - stage.h * 0.054 * configuration.combo.scale;
            var comboTextWidth = 0 * configuration.combo.scale;
            var comboTextHeight = stage.h * 0.017 * configuration.combo.scale;
            var comboTextX = 0;
            var comboTextY = stage.t - stage.h * 0.0985 * configuration.combo.scale;
            setUI(
                ui.comboValue,
                comboValueX, comboValueY, 
                0.5, 0.5, 
                comboValueWidth, comboValueHeight, 
                0, 
                configuration.combo.alpha, 
                HorizontalAlign.Center, 
                false
            );
            setUI(
                ui.comboText,
                comboTextX, comboTextY, 
                0.5, 0.5, 
                comboTextWidth, comboTextHeight, 
                0, 
                configuration.combo.alpha, 
                HorizontalAlign.Center, 
                false
            ); 
        }
        
        // 进度条参数
        var progressWidth = stage.w - interfaceGap * 2;
        var progressHeight = 0.15 * configuration.progress.scale;
        var progressX = stage.l + interfaceGap;
        var progressY = stage.b + interfaceGap;
        setUI(
            ui.progress,
            progressX, progressY, 
            0, 0, 
            progressWidth, progressHeight, 
            0, 
            configuration.progress.alpha, 
            HorizontalAlign.Center, 
            true
        );
        
        setScore(score.perfect, score.great, score.good);
        // buckets[Buckets.note].set(-1 * judgment.perfect * 1000, judgment.perfect * 1000, -1 * judgment.great * 1000, judgment.great * 1000, -1 * judgment.good * 1000, judgment.good * 1000);
        // buckets[Buckets.drag].set(-1 * judgment.good * 1000, judgment.good * 1000, -1 * judgment.good * 1000, judgment.good * 1000, -1 * judgment.good * 1000, judgment.good * 1000);
        // buckets[Buckets.hold].set(-1 * judgment.perfect * 1000, judgment.perfect * 1000, -1 * judgment.good * 1000, judgment.good * 1000, -1 * judgment.good * 1000, judgment.good * 1000);
        // buckets[Buckets.flick].set(-1 * judgment.good * 1000, judgment.good * 1000, -1 * judgment.good * 1000, judgment.good * 1000, -1 * judgment.good * 1000, judgment.good * 1000);

        combo = 0;
        judgeStatus = If(hasIndicator, 2, 0);
        maxCombo = 0;
        notes = 0;
        accscore = 0;
    }

	SonolusApi spawnTime() { return 0; }
	SonolusApi despawnTime() { return 0; }
};