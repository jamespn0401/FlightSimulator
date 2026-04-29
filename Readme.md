# Flight Computer Simulator

A C++ simulation engine producing live telemetry into a backend platform with APIs, storage, replay, anomaly detection, dashboards, and cloud deployment.

## Architecture

[C++ Flight Engine] *

   |
   
   v
   
Telemetry Stream (JSON / Protobuf) **

   |
   
   v
   
Ingestion Service (Python/FastAPI or Go)

   |
   
   +--> PostgreSQL / Timeseries DB
   
   +--> Redis Cache
   
   +--> Queue (Kafka / RabbitMQ optional)
   
   |
   
   v
   
React Dashboard

   |
   
   v
   
Replay / Analytics / Alerts

*Current
**Next
