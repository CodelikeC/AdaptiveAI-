use crate::traits::*;
use crate::config;
use anyhow::Result;
use std::sync::{Arc, Mutex};
use arc_swap::ArcSwap;
use log::info;

pub struct BehaviorEngine {
    config: Arc<ArcSwap<config::ModuleConfig>>,
    learned_rules: Vec<String>,
}

impl BehaviorEngine {
    pub fn new(config: Arc<ArcSwap<config::ModuleConfig>>) -> Self {
        BehaviorEngine {
            config,
            learned_rules: Vec::new(),
        }
    }
}

impl BehaviorModule for BehaviorEngine {
    fn analyze(&self, context: &str) -> String {
        let cfg = self.config.load();
        let mut result = String::new();
        for rule in cfg.behavior_rules.iter().chain(&self.learned_rules) {
            if context.contains(rule) {
                result.push_str(&format!("[BehaviorEngine] Detected rule '{}': {}\n", rule, context));
            }
        }
        if result.is_empty() {
            "[BehaviorEngine] No significant behavior detected".to_string()
        } else {
            result
        }
    }
}

impl Learnable for BehaviorEngine {
    fn learn(&mut self, data: &str) -> Result<()> {
        info!("Learning new behavior rule from data: {}", data);
        if !data.is_empty() && !self.learned_rules.contains(&data.to_string()) {
            self.learned_rules.push(data.to_string());
            Ok(())
        } else {
            Err(anyhow::anyhow!("No new rule to learn"))
        }
    }

    fn save_state(&self) -> String {
        serde_json::to_string(&self.learned_rules).unwrap_or_default()
    }

    fn load_state(&mut self, state: &str) -> Result<()> {
        self.learned_rules = serde_json::from_str(state)?;
        Ok(())
    }
}
