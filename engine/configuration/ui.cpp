using namespace std;

EngineConfigurationUI configurationUI = {
    .primaryMetric = Metric.arcade, 
    .secondaryMetric = Metric.life,
    .menuVisibility = {
        .scale = 1.0,
        .alpha = 1.0
    }, 
    .judgmentVisibility = {
        .scale = 1.0,
        .alpha = 1.0
    },
    .comboVisibility = {
        .scale = 1.0,
        .alpha = 1.0
    },
    .primaryMetricVisibility = {
        .scale = 1.0,
        .alpha = 1.0
    },
    .secondaryMetricVisbility = {
        .scale = 1.0,
        .alpha = 1.0
    },
    .progressVisibility = {
    	.scale = 1.0,
    	.alpha = 1.0	
    },
    .tutorialNavigationVisibility = {
        .scale = 1.0,
        .alpha = 1.0
    },
    .tutorialInstructionVisibility = {
        .scale = 1.0,
        .alpha = 1.0
    },
    .judgmentAnimation = {
        .scale = {
            .from = 0.0,
            .to = 0.0,
            .duration = 0.2,
            .ease = AnimationEase.InSine
        },
        .alpha = {
            .from = 0.0,
            .to = 0.0,
            .duration = 0.2,
            .ease = AnimationEase.InSine
        }
    },
    .comboAnimation = {
        .scale = {
            .from = 0.2,
            .to = 1.0,
            .duration = 0.2,
            .ease = AnimationEase.InSine
        },
        .alpha = {
            .from = 0.2,
            .to = 1.0,
            .duration = 0.2,
            .ease = AnimationEase.Linear
        }
    },
    .judgmentErrorStyle = ErrorStyle.none,
    .judgmentErrorPlacement = ErrorPlacement.both,
    .judgmentErrorMin = 20,
    .scope = Scope
};
