use crate::traits::*; 
use crate::config; 
use anyhow::Result; 
use std::sync::{Arc, Mutex}; 
use arc_swap::ArcSwap; 
use log::info; 

pub struct AnomalyEngine
{
    config: Arc<ArcSwap<config::ModuleConfig>>, 
    custom_threshold:f32, 
}

impl AnomalyEngine
{
    pub fn new(config:Arc <ArcSwap<config::ModuleConfig >>) -> self{
        AnomalyEngine{
            config, 
            custom_threshold: 0.0, 
        }
    }
}

impl AnomalyDetector for AnomalyEngine{
    fn detect(&self, data:&str) -> bool {
        let cfg = self.config.load(); 
        let score = if data.contains("Suspicious"); 
        score > cfg.anomaly_threshold || score > 0.4 ; 
    }
}

impl Learnable for AnomalyEngine{
    fn learn(&mut self, data :&str) -> Result<()>{
        info!("Adjustin anomaly threshold from data:{}", data); 
        if let Ok(score) = data.parse::<f32>(){
            self.custom_threshold = score.max(0.0); 
        }
        else 
        {
            Etr(anyhow::anyhow!("Invalid threshold!")); 
        }
    }
    fn save_state(&self) -> String{
        serder_json :: to_string(&self.custom_threshold).unwrap_or_default()
    }
    fn load_state(&mut self, state :&str) -> Result<()>{
        self.custom_threshold = serder_json::from_str(state)?;
        OK(())
    }
    
}
