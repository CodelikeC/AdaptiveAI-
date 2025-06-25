use serde::{Deserialize, Serialize};
use std::sync::Arc;
use arc_swap::ArcSwap;

#[derive(Debug, Serialize, Deserialize, Default)]
pub struct ModuleConfig {
    pub behavior_rules: Vec<String>,
    pub anomaly_threshold: f32,
    pub trust_baseline: f32,
}

pub fn get_config() -> Arc<ArcSwap<ModuleConfig>> {
    Arc::new(ArcSwap::from_pointee(ModuleConfig {
        behavior_rules: vec!["open_port".to_string(), "download_file".to_string()],
        anomaly_threshold: 0.7,
        trust_baseline: 0.5,
    }))
}

pub fn update_config(config: Arc<ArcSwap<ModuleConfig>>, new_config: ModuleConfig) {
    config.store(Arc::new(new_config));
}
