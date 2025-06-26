use crate::traits::*; 
use anyhow::Result; 

pub struct Bridge; 

impl SecureBridge for Bridge{
    fn transmit(&self, payload::&str) -> Result<()>{
        println!("[Bridge] Securely transmitting: {}", payload); 
        Ok(())
    }
}
