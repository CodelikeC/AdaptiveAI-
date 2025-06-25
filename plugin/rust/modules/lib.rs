pub mod traits;
pub mod config;
pub mod modules;

use anyhow::Result; 
use tokio; 

#[Tokio :: main]
pub async fn run_plugins () -> Result<()>{
    env_logger::init();
    let config = modules::config::get_config(); 
    let behavior = Arc:: new(Mutex:: new(modules::behavior))
    let anomaly  = Arc::new(Mutex::new(modules::anomaly))
    let trust = Arc :: new(Mutex :: new(modules::trust))

    // learn a new logic:
    {
        let mut behavior = behavior.lock().unwrap();
        behavior.learn("new_rule")?;
    }

    {
        let mut anomaly = anomaly.lock().unwrap();
        anomaly.learn("0.8")?; 
    }

    {
        let mut trust  = trust.lock().unwrap();
        trust.learn("external_node:0.7")?; 
    }

    // Running the analysis ..//
    let behavior_result = behavior.lock().unwrap().analysis("User opened port 8080"); 
    println!("{}", behavior_result); 
    
    let suspicious = anomaly.lock().unwrap().detect("User downloaded suspicious file");
    println!("Anomaly detected {}", suspicious); 

    let trust_score = trust.lock().unwrap().validate(""); 
    println!("Trust Score: {:.2}", trust_score);

    let bridge = modules::bridge::Bridge;
    bridge.transmit("data: encrypted-logic-stream")?; 
    let updater = modules::updater::Updater;
    let sync_msg = updater.sync("node-12.domain.internal")?;
    println!("{}", sync_msg);

    OK(())
}