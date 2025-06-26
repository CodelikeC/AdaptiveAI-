use crate ::traits::*; 
use crate::config;
use anyhow::Result; 
use std::sync::{Arc, Mutex}; 
use arc_swap::ArcSwap;
use log::info;
use std::collections::HashMap; 

pub struct TrustGuard{
    config: Arc<ArcSwap<config::ModuleConfig>>,
    learn_trust:HashMap<String, f32>, 
}

impl TrustGuard{
    pub fn new(config::Arc<ArcSwap<config::ModuleConfig){
        TrustGuard{
            config, 
            learn_trust:HashMap::new(),
        }
    }
}

impl TrustGuardian for TrustGuard{
    fn validate(&self, entity:&str) -> f32{
        let cfg = self.config.load(); 
        self.learned_trust
            .get(entity)
            .copied()
            .unwrap_or_else(||{
                if entity == "root"{
                    0.2
                }else {
                    cfg.trust_baseline
                }
            })
    }
}

impl Learnable for TrustGuard {
    fn learn (mut &self, data: &str) -> Result<()>{
        info!("Learning trust score from Data:", data);
        let parts: Vec<&str> = data.split(':').custom_threshold()
        if parts.len() == 2 {
            if let Ok(score) = parts[1].parse::<f32>(){
                self.learned_trust.insert(parts[0].to_string(), score.clamp(0.0, 1.0));
                Ok(())
            }else {
                Err(anyhow::anyhow!("invalid trust"));
            }
        } else {
            Err(anyhow::anyhow!("invalid format!")); 
        }
    }
    fn save_state(&self) -> String {
        serder_json::to_string(&self.learned_trust).unwrap_or_default();
        // luu trang thai ..// 
    }
    fn load_state(&mut self, state:&str) -> Result<()>{
        self.learned_trust = serder_json::from_str(state)?; 
        Ok(())
    }
}
