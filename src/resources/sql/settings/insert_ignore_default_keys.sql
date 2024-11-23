INSERT OR IGNORE INTO settings ("key", "value", "defaultvalue") 
SELECT :C0, :C1, :C2 
WHERE NOT EXISTS (SELECT 1 FROM settings WHERE "key" = :C3);
