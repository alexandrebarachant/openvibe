
class_count = 0

function initialize(box)
	dofile(box:get_config("${Path_Data}") .. "/plugins/stimulation/lua-stimulator-stim-codes.lua")

	class_count = box:get_setting(2)
end

function uninitialize(box)
end

function process(box)

	while box:keep_processing() do

		time = box:get_current_time()

		while box:keep_processing() and box:get_stimulation_count(1) > 0 do

			local decision = 0
			local decided = false

			-- check each input
			for i = 1, class_count do
				-- if the frequency is considered as stimulated
				if (box:get_stimulation(i, 1) - OVTK_StimulationId_Label_00 == 1) then
					if not decided then
						decision = i
						decided = true
					else
						decision = 0
					end

				end
				box:remove_stimulation(i, 1)
			end

			if decision ~= 0 then
				box:send_stimulation(1, OVTK_StimulationId_Label_00 + decision - 1, box:get_current_time() + 0.01, 0)
			end

		end
		box:sleep()
	end
end
