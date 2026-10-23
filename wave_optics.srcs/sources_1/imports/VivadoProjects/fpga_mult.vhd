----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 10/20/2026 12:33:59 AM
-- Design Name: 
-- Module Name: fpga_mult - rtl
-- Project Name: 
-- Target Devices: 
-- Tool Versions: 
-- Description: 
-- 
-- Dependencies: 
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
-- 
----------------------------------------------------------------------------------


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity fpga_mult is
  Port ( 
    clk : in std_logic;
    reset : in std_logic;
    start_read : in std_logic;
    
    operand_a : in std_logic_vector(15 downto 0);
    operand_b : in std_logic_vector(15 downto 0);
    
    result : out std_logic_vector(31 downto 0);
    busy : out std_logic;
    done : out std_logic
  );
end fpga_mult;

architecture rtl of fpga_mult is
    type CALC_STATE is (IDLE, WORKING);
    signal state : CALC_STATE := IDLE;
    
    signal r_operand_a : signed(15 downto 0) := (others => '0');
    signal r_operand_b : signed(15 downto 0) := (others => '0');
    
    signal r_result : signed(31 downto 0) := (others => '0');
    signal r_start_read : std_logic := '0';
    signal r_busy : std_logic := '0';
    signal r_done : std_logic := '0';
begin
    result <= std_logic_vector(r_result);
    busy <= r_busy;
    done <= r_done;
    process(clk)
    begin
        if rising_edge(clk) then
            if reset = '1' then
                state <= IDLE;
                r_operand_a <= (others => '0');
                r_operand_b <= (others => '0');
                r_result <= (others => '0');
                r_start_read <= '0';
                r_busy <= '0';
                r_done <= '0';
            else
                r_start_read <= start_read;
                case state is
                    when IDLE =>
                        if start_read = '1' and r_start_read = '0' then
                            r_busy <= '1';
                            r_operand_a <= signed(operand_a);
                            r_operand_b <= signed(operand_b);
                            r_done <= '0';
                            state <= WORKING;
                        end if; --wait for start signal
                        --start signal should originate from Zynq PS in the same Clock Domain so should be stable.
                    when WORKING =>
                        r_result <= r_operand_a * r_operand_b;--Q2.30
                        r_busy <= '0';
                        r_done <= '1';
                        state <= IDLE;
                end case;
            end if;
        end if;
    end process;
end rtl;
