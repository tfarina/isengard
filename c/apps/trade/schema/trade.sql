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

insert into stockinfo (symbol, name, sector, subsector, segment) values ('CHKE', 'CHESAPEAKE', 'Petrleo, Gs e Biocombustveis', 'Petrleo, Gs e Biocombustveis', 'Explorao e/ou Refino');
