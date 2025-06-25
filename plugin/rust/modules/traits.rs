use anyhow::Result;
use serde::{Deserialize, Serialize};
use std::sync::{Arc, Mutex};
use arc_swap::ArcSwap;

pub trait Learnable {
    fn learn(&mut self, data: &str) -> Result<()>;
    fn save_state(&self) -> String;
    fn load_state(&mut self, state: &str) -> Result<()>;
}

pub trait BehaviorModule {
    fn analyze(&self, context: &str) -> String;
}

pub trait AnomalyDetector {
    fn detect(&self, data: &str) -> bool;
}

pub trait TrustGuardian {
    fn validate(&self, entity: &str) -> f32;
}

pub trait SecureBridge {
    fn transmit(&self, payload: &str) -> Result<()>;
}

pub trait SyncUpdater {
    fn sync(&self, remote: &str) -> Result<String>;
}