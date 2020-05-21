-- CREATE Order
-- 1. - node locations
-- 2. - node hosts
--    - node floors
--    - node nodes
-- 3. - node gateways
--    - node repeaters
--    - node displays
--    - node devices
-- 4. - node statuses
--    - node batteries
-- 5. - node summaries
--
-- DROP Order
DROP TRIGGER IF EXISTS `onspotmy_node_db`.`insert_summary`;
DROP TABLE IF EXISTS `onspotmy_node_db`.`node_summaries`;
DROP TABLE IF EXISTS `onspotmy_node_db`.`node_resets`;
DROP TABLE IF EXISTS `onspotmy_node_db`.`node_batteries`;
DROP TABLE IF EXISTS `onspotmy_node_db`.`node_statuses`;
DROP TABLE IF EXISTS `onspotmy_node_db`.`node_devices`;
DROP TABLE IF EXISTS `onspotmy_node_db`.`node_displays`;
DROP TABLE IF EXISTS `onspotmy_node_db`.`node_repeaters`;
DROP TABLE IF EXISTS `onspotmy_node_db`.`node_gateways`;
DROP TABLE IF EXISTS `onspotmy_node_db`.`node_nodes`;
DROP TABLE IF EXISTS `onspotmy_node_db`.`node_floors`;
DROP TABLE IF EXISTS `onspotmy_node_db`.`node_hosts`;
DROP TABLE IF EXISTS `onspotmy_node_db`.`node_locations`;