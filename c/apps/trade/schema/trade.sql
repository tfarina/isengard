CREATE TABLE `historicaldata` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `symbol` char(6) NOT NULL,
  `date` date NOT NULL,
  `open` decimal(18,4) NOT NULL,
  `high` decimal(18,4) NOT NULL,
  `low` decimal(18,4) NOT NULL,
  `close` decimal(18,4) NOT NULL,
  `adjClose` decimal(18,4) NOT NULL,
  `volume` int(11) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

insert into historicaldata (symbol, date, open, high, low, close, adjClose, volume) values ('UNIP6', '2018-02-07', '22.40', '22.50', '21.82', '22.50', '22.50', '112500');

CREATE TABLE `stockinfo` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `symbol` char(6) NOT NULL,
  `name` varchar(255) NOT NULL,
  `sector` varchar(255) NOT NULL,
  `subsector` varchar(255) NOT NULL,
  `segment` varchar(255) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

insert into stockinfo (symbol, name, sector, subsector, segment) values ('CHKE', 'CHESAPEAKE', 'Petroleo, Gas e Biocombustveis', 'Petroleo, Gas e Biocombustveis', 'Exploracao e/ou Refino');
insert into stockinfo (symbol, name, sector, subsector, segment) values ('CHVX', 'CHEVRON', 'Petroleo, Gas e Biocombustveis', 'Petroleo, Gas e Biocombustiveis', 'Exploracao e/ou Refino');
insert into stockinfo (symbol, name, sector, subsector, segment) values ('COPH', 'COPHILLIPS', 'Petroleo, Gas e Biocombustveis', 'Petroleo, Gas e Biocombustiveis', 'Exploracao e/ou Refino');
insert into stockinfo (symbol, name, sector, subsector, segment) values ('EXXO', 'EXXON MOBIL', 'Petroleo, Gas e Biocombustveis', 'Petroleo, Gas e Biocombustiveis', 'Exploracao e/ou Refino');
insert into stockinfo (symbol, name, sector, subsector, segment) values ('OGSA', 'NOVA OLEO', 'Petroleo, Gas e Biocombustveis', 'Petroleo, Gas e Biocombustiveis', 'Exploracao e/ou Refino');
insert into stockinfo (symbol, name, sector, subsector, segment) values ('OGXP', 'OGX PETROLEO', 'Petroleo, Gas e Biocombustveis', 'Petroleo, Gas e Biocombustiveis', 'Exploracao e/ou Refino');
insert into stockinfo (symbol, name, sector, subsector, segment) values ('RPMG', 'PET MANGUINH', 'Petroleo, Gas e Biocombustveis', 'Petroleo, Gas e Biocombustiveis', 'Exploracao e/ou Refino');
insert into stockinfo (symbol, name, sector, subsector, segment) values ('PETR', 'PETROBRAS', 'Petroleo, Gas e Biocombustveis', 'Petroleo, Gas e Biocombustiveis', 'Exploracao e/ou Refino');
insert into stockinfo (symbol, name, sector, subsector, segment) values ('PRIO', 'PETRORIO', 'Petroleo, Gas e Biocombustveis', 'Petroleo, Gas e Biocombustiveis', 'Exploracao e/ou Refino');
insert into stockinfo (symbol, name, sector, subsector, segment) values ('QGEP', 'GEP PART', 'Petroleo, Gas e Biocombustveis', 'Petroleo, Gas e Biocombustiveis', 'Exploracao e/ou Refino');
insert into stockinfo (symbol, name, sector, subsector, segment) values ('SANC', 'SANCHEZ ENER', 'Petroleo, Gas e Biocombustveis', 'Petroleo, Gas e Biocombustiveis', 'Exploracao e/ou Refino');
insert into stockinfo (symbol, name, sector, subsector, segment) values ('TSOR', 'TESORO CORP', 'Petroleo, Gas e Biocombustveis', 'Petroleo, Gas e Biocombustiveis', 'Exploracao e/ou Refino');
insert into stockinfo (symbol, name, sector, subsector, segment) values ('RIGG', 'TRANSOCEAN', 'Petroleo, Gas e Biocombustveis', 'Petroleo, Gas e Biocombustiveis', 'Exploracao e/ou Refino');
insert into stockinfo (symbol, name, sector, subsector, segment) values ('VLOE', 'VALERO ENER', 'Petroleo, Gas e Biocombustveis', 'Petroleo, Gas e Biocombustiveis', 'Exploracao e/ou Refino');
insert into stockinfo (symbol, name, sector, subsector, segment) values ('YPFL', 'Y P F', 'Petroleo, Gas e Biocombustveis', 'Petroleo, Gas e Biocombustiveis', 'Exploracao e/ou Refino');
