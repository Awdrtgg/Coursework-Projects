LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE IEEE.STD_LOGIC_UNSIGNED.ALL;

ENTITY music IS
	PORT(CP,CP_CM,CP_AH,CP_AM:IN STD_LOGIC;
		 H1,H2,M1,M2,S2: IN STD_LOGIC_VECTOR(3 DOWNTO 0);
		 STOP: IN STD_LOGIC;
		 S1: IN STD_LOGIC_VECTOR(6 DOWNTO 0);
		 Z: OUT STD_LOGIC);
END ENTITY;

ARCHITECTURE spkout OF music IS
	SIGNAL MM1, SS2: STD_LOGIC_VECTOR(3 DOWNTO 0);
	SIGNAL STA1:STD_LOGIC;
	SIGNAL STA2:STD_LOGIC;
	SIGNAL STA3:STD_LOGIC;
	BEGIN	
		MM1 <= M1;
		SS2 <= S2;
		STA1 <= '1' WHEN ((CP_CM = '1') AND (MM1 = "0000") AND (SS2 = "0000")) ELSE
				'0';
		STA2 <= '1' WHEN (CP_AH = '1' AND CP_AM = '1') ELSE
				'0';		
				
		PROCESS(CP)
			VARIABLE COUNT: INTEGER RANGE 0 TO 150;
		BEGIN
			IF RISING_EDGE(CP) THEN
				IF (COUNT = 150) THEN
					COUNT := 0;
					STA3 <= NOT STA3;
				ELSE
					COUNT := COUNT + 1;
				END IF;
			END IF;
		END PROCESS;		
		
		Z<= (STA1 OR (STA2 AND NOT STOP) ) AND STA3 AND CP;
END spkout;