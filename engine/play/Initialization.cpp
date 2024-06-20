class Initialization: public Archetype {
    public:

    static constexpr const char* name = "Phigros Initialization";
    bool hasInput = false;

    SonolusApi preprocess() {
    	FUNCBEGIN
        // 界面开关参数
        let menuWidth = 0.15 * ui.menuConfiguration.scale;
        let menuHeight = 0.15 * ui.menuConfiguration.scale;
        let menuX = stage.l + 0.05;
        let menuY = stage.t - 0.05;
        ui.menu.set(menuX, menuY, 0, 1, menuWidth, menuHeight, 0, ui.menuConfiguration.alpha, HorizontalAlign.Center, true);
        
        // 首要指标参数
        // let primaryMetricBarWidth = 0.75 * ui.primaryMetricConfiguration.scale;
        // let primaryMetricBarHeight = 0.15 * ui.primaryMetricConfiguration.scale;
        // let primaryMetricBarX = screen.l + interfaceGap;
        // let primaryMetricBarY = screen.t - interfaceGap;
        // let primaryMetricValueWidth = 0 * ui.primaryMetricConfiguration.scale;
        // let primaryMetricValueHeight = 0.08 * ui.primaryMetricConfiguration.scale;
        // let primaryMetricValueX = primaryMetricBarX + 0.715 * ui.primaryMetricConfiguration.scale;
        // let primaryMetricValueY = primaryMetricBarY - 0.035 * ui.primaryMetricConfiguration.scale;
        // ui.primaryMetricBar.set(primaryMetricBarX, primaryMetricBarY, 0, 1, primaryMetricBarWidth, primaryMetricBarHeight, 0, ui.primaryMetricConfiguration.alpha, HorizontalAlign.Left, true);
        // ui.primaryMetricValue.set(primaryMetricValueX, primaryMetricValueY, 1, 1, primaryMetricValueWidth, primaryMetricValueHeight, 0, ui.primaryMetricConfiguration.alpha, HorizontalAlign.Right, false);
        
        // 次要指标参数
        // let secondaryMetricBarWidth = 0.55 * ui.secondaryMetricConfiguration.scale;
        // let secondaryMetricBarHeight = 0.15 * ui.secondaryMetricConfiguration.scale;
        // let secondaryMetricBarX = screen.r - interfaceGap - menuWidth - interfaceGap;
        // let secondaryMetricBarY = screen.t - interfaceGap;
        // let secondaryMetricValueWidth = 0 * ui.secondaryMetricConfiguration.scale;
        // let secondaryMetricValueHeight = 0.08 * ui.secondaryMetricConfiguration.scale;
        // let secondaryMetricValueX = secondaryMetricBarX - 0.035 * ui.secondaryMetricConfiguration.scale;
        // let secondaryMetricValueY = secondaryMetricBarY - 0.035 * ui.secondaryMetricConfiguration.scale;
        // ui.secondaryMetricBar.set(secondaryMetricBarX, secondaryMetricBarY, 1, 1, secondaryMetricBarWidth, secondaryMetricBarHeight, 0, ui.secondaryMetricConfiguration.alpha, HorizontalAlign.Left, true);
        // ui.secondaryMetricValue.set(secondaryMetricValueX, secondaryMetricValueY, 1, 1, secondaryMetricValueWidth, secondaryMetricValueHeight, 0, ui.secondaryMetricConfiguration.alpha, HorizontalAlign.Right, false);
        
        // 连击指标参数
        // let comboValueWidth = 0 * ui.comboConfiguration.scale;
        // let comboValueHeight = screen.h * 0.08 * ui.comboConfiguration.scale;
        // let comboValueX = 0;
        // let comboValueY = 0.8;
        // let comboTextWidth = 0 * ui.comboConfiguration.scale;
        // let comboTextHeight = screen.h * 0.033 * ui.comboConfiguration.scale;
        // let comboTextX = screen.w * 0.4;
        // let comboTextY = Lerp(t, b, 0.425);
        // ui.comboValue.set(comboValueX, comboValueY, 0.5, 0.5, comboValueWidth, comboValueHeight, 0, ui.comboConfiguration.alpha, HorizontalAlign.Center, false);
        // ui.comboText.set(comboTextX, comboTextY, 0.5, 2.75, comboTextWidth, comboTextHeight, 0, ui.comboConfiguration.alpha, HorizontalAlign.Center, false);
        
        LevelScore.set(0, score.perfect);
        LevelScore.set(1, score.great);
        LevelScore.set(2, score.good);
        EntityDespawn.set(0, 1);
        return VOID;
    }
    SonolusApi spawnOrder() { return 0; }
    SonolusApi shouldSpawn() { return 1; }
};