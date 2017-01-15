vedis = require 'vedis'

local vedisdb = vedis.open('test.vedis')
vedis.begin(vedisdb)
vedis.store(vedisdb, 'foo', 'bar')
vedis.commit(vedisdb)

local result = vedis.fetch(vedisdb, 'foo')
print(result)
