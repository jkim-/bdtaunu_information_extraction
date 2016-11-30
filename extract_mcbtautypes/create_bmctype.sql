BEGIN;
CREATE TYPE bmctype AS ENUM (
  'NoB',
  'Dtau',
  'Dstartau',
  'Dl',
  'Dstarl',
  'Dstarstar_res',
  'Dstarstar_nonres',
  'SL',
  'Had',
  'null'
);

CREATE TYPE taumctype AS ENUM (
  'NoTau',
  'tau_e',
  'tau_mu',
  'tau_k',
  'tau_h',
  'null'
);
END;
