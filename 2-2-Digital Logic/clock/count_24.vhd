LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE IEEE.STD_LOGIC_UNSIGNED.ALL;

ENTITY count_24 IS
	PORT(CLR_24: IN STD_LOGIC;
		CLK_24: IN STD_LOGIC;
		CP_FLASH: IN STD_LOGIC;
		SET_24: IN STD_LOGIC;
		SET_A: IN STD_LOGIC;
		CP_ADD: IN STD_LOGIC;
		SETING: IN STD_LOGIC;
		N2,N1: OUT STD_LOGIC_VECTOR(3 DOWNTO 0);
		C: OUT STD_LOGIC;
		A: OUT STD_LOGIC);
END ENTITY;

ARCHITECTURE count_24_FUN OF count_24 IS
	SIGNAL T2,T1,A2,A1,FLASH: STD_LOGIC_VECTOR(3 DOWNTO 0);
	SIGNAL CP, CN, AN: STD_LOGIC;
	BEGIN
		CP <= 	CLK_24 WHEN SETING = '0' ELSE 
				SET_24 WHEN SET_24 = '0' ELSE
								CP_ADD;
		FLASH <= "1111" WHEN (CP_FLASH = '1' AND SET_24 = '1') ELSE "0000";
		
		PROCESS(CP, CLR_24)
		BEGIN
			IF T2 = A2 AND T1 = A1 AND SETING = '0' THEN
				AN <= '1';
			ELSE
				AN <= '0';
			END IF;
			
			IF CLR_24 = '1' THEN
				T1 <= "0000";
				T2 <= "0000";
				A1 <= "0000";
				A2 <= "0000";
				CN <= '0';
				AN <= '0';
			ELSIF RISING_EDGE(CP) THEN
				IF T2 = 2 AND T1 = 3 AND SETING = '0' THEN
					CN <= '1';
				ELSE
					CN <= '0';
				END IF;
				
				IF SET_A = '1' THEN
					IF A1 = 9 THEN
						A1 <= "0000";
						A2 <= A2 +1;
					ELSIF A2 = 2 AND A1 = 3 THEN
						A1 <= "0000";
						A2 <= "0000";
					ELSE
						A1 <= A1 +1;
					END IF;
				ELSE
					IF T1 = 9 THEN
						T1 <= "0000";
						T2 <= T2 +1;
					ELSIF T2 = 2 AND T1 = 3 THEN
						T1 <= "0000";
						T2 <= "0000";
					ELSE
						T1 <= T1 +1;
					END IF;
				END IF;
			END IF;	
		END PROCESS;
		
		N2 <= (T2 OR FLASH) WHEN (SETING = '1' AND SET_A = '0') ELSE 
				(A2 OR FLASH) WHEN (SETING = '1' AND SET_A = '1') ELSE
														T2;
		N1 <= (T1 OR FLASH) WHEN (SETING = '1' AND SET_A = '0') ELSE 
				(A1 OR FLASH) WHEN (SETING = '1' AND SET_A = '1') ELSE
														T1;
		C <= CN;
		A <= AN;
END count_24_FUN;