-- REFERENCE:

-- FAU :	J. Echavarria, S. Wildermann, A. Becher, J. Teich, D. Ziener, "FAU: 
--			Fast and Error-Optimized Approximate Adder Units on LUT-Based FPGAs,"
--			in proceedings of the 2016 International Conference on Field-Programmable
--			Technology (FPT)


-- PARAMETERS:

-- N = bit width (N must be greater than 1)
-- M = splitting point (M must be greater than 1)
-- P = prediction bits (P must be greater than 1)

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity FAU is
    generic(N : integer;
            M : integer;
            P : integer);
            
    port(In0 : in  unsigned (N-1 downto 0);
         In1 : in  unsigned (N-1 downto 0);
         res : out unsigned (N   downto 0));
   
    attribute dont_touch : string;
--  attribute use_dsp48  : string;
      
    attribute dont_touch of FAU : entity is "true";
--  attribute use_dsp48  of FAU : entity is "no";
    
end FAU;

architecture Behavioral of FAU is
begin
    
    process (In0, In1) 
        variable MSP : unsigned(N-M downto 0) := (others => '0');
        variable LSP : unsigned(M   downto 0) := (others => '0');
        variable sha : unsigned(P   downto 0) := (others => '0');
    begin
        
        assert N > 1 report "Parameter N must be greater than 1" severity failure;
        assert M > 1 report "Parameter M must be greater than 1" severity failure;
        assert P > 1 report "Parameter P must be greater than 1" severity failure;
        
        MSP := resize(In0(N-1 downto M  ), MSP'length) + resize(In1(N-1 downto M  ), MSP'length);
        LSP := resize(In0(M-1 downto 0  ), LSP'length) + resize(In1(M-1 downto 0  ), LSP'length);
        sha := resize(In0(M-1 downto M-P), sha'length) + resize(In1(M-1 downto M-P), sha'length);
        
        if(sha(P) = '1') then           -- Carry prediction
            MSP := MSP + 1;
        elsif(LSP(M) = '1') then        -- Error magnitude reduction
            LSP := (others => '1');
        end if;
        
        res(N   downto M) <= MSP(N-M downto 0);
        res(M-1 downto 0) <= LSP(M-1 downto 0);
        
    end process;
           
end Behavioral;
