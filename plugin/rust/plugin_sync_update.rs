use crate::traits::*; 
use anyhow::Result; 

pub struct Updater; 

impl SyncUpdater for Updater{
    fn sync(&self, remote:&str) -> Result<String>{
        Ok(format!("Synced with remote node :{}", remote))
    }
}
