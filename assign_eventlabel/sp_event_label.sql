-- This procedure determines the event type of
-- a record given the types of the B's and tau's
--
-- Example usage:
-- SELECT sp_label_events(b1_mctype, b2_mctype, b1tau_mctype, b2tau_mctype)
-- FROM bmctypes_generic; 
--
-- Sanity checks:
--
-- SELECT * 
-- FROM bmctypes_generic 
-- WHERE CAST(b1_mctype AS text) LIKE '%tau%' AND 
-- CAST(b1tau_mctype AS text) = 'NoTau';
--
-- should return empty set (same with b1 <-> b2)


CREATE OR REPLACE FUNCTION sp_label_events(
  IN b1   bmctype,
  IN b2   bmctype,
  IN b1tau taumctype,
  IN b2tau taumctype
) RETURNS INTEGER AS
$$
-- Declare variables
DECLARE
dtau     BOOLEAN := (b1 = 'Dtau' OR b2 = 'Dtau');
dstartau BOOLEAN := (b1 = 'Dstartau' OR b2 = 'Dstartau');
tau_h    BOOLEAN := (b1tau = 'tau_k' OR b1tau = 'tau_h' OR
                     b2tau = 'tau_k' OR b2tau = 'tau_h');
tau_l    BOOLEAN := (b1tau = 'tau_e' OR b1tau = 'tau_mu' OR
                     b2tau = 'tau_e' OR b2tau = 'tau_mu');

BEGIN
-- Decide whether event is signal
IF (dtau = TRUE) AND (tau_l = TRUE) THEN
  RETURN 1; -- dtau
ELSEIF (dstartau = TRUE) AND (tau_l = TRUE) THEN
  RETURN 2; -- dstartau
END IF;

-- Check if it's normalization
IF (b1 = 'Dl') OR (b2 = 'Dl') THEN
  RETURN 3; -- dl
ELSEIF (b1 = 'Dstarl') OR (b2 = 'Dstarl') THEN
  RETURN 4; -- dstarl
END IF;

-- Determine which background type it is
IF (b1 = 'NoB') OR (b2 = 'NoB') THEN
  RETURN 7; -- cont
ELSEIF (b1 = 'SL') OR (b1 = 'Dstarstar_res') OR (b1 = 'Dstarstar_nonres') OR
       (b2 = 'SL') OR (b2 = 'Dstarstar_res') OR (b2 = 'Dstarstar_nonres') THEN
  RETURN 5; -- SL
END IF;

RETURN 6; -- had

END;
$$ LANGUAGE plpgsql;

-- 1 : dtau
-- 2 : dstartau
-- 3 : dl
-- 4 : dstarl
-- 5 : SL
-- 6 : had
-- 7 : cont
-- -1 : bad
